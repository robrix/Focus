// FParser.c
// Created by Rob Rix on 2010-10-04
// Copyright 2010 Monochrome Industries

#include "Prototypes/FMessagePrototype.h"
#include "Prototypes/FListNodePrototype.h"
#include "Prototypes/FFunctionPrototype.h"
#include "FParser.h"
#include "FSymbol.h"
#include "Prototypes/FFunctionPrototype.h"
#include "FObject.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool FParseCharacterSet(const char *source, size_t index, const char *characters, size_t *outLength) {
	size_t length = 0;
	if(strlen(source) > index) {
		length = strspn(source + index, characters);
	}
	if(outLength && length) *outLength = length;
	return length > 0;
}

bool FParseToken(const char *source, size_t index, const char *token) {
	char *start = 0;
	if(strlen(source) > index) {
		start = strstr(source + index, token);
	}
	return start == source + index;
}


bool FParseWord(const char *source, size_t index, size_t *outLength) {
	size_t first = 0, rest = 0;
 	bool result =
		FParseCharacterSet(source, index, "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", &first)
	&&	(FParseCharacterSet(source, index + first, "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890", &rest) || 1);
	if(outLength && result) *outLength = first + rest;
	return result;
}

bool FParseWhitespace(const char *source, size_t index, size_t *outLength) {
	return FParseCharacterSet(source, index, " \t\v", outLength);
}

bool FParseWhitespaceAndNewlines(const char *source, size_t index, size_t *outLength) {
	return FParseCharacterSet(source, index, " \n\r\t\v\f", outLength);
}


bool FParseKeyword(const char *source, size_t index, size_t *outLength) {
	size_t wordLength = 0;
	bool result =
		(FParseWord(source, index, &wordLength) || 1)
	&&	FParseToken(source, index + wordLength, ":");
	if(outLength && result) *outLength = wordLength + 1;
	return result;
}


bool FParseNullaryMessage(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode) {
	size_t length = 0;
	bool result = FParseWord(source, index, &length) && !FParseToken(source, index + length, ":");
	if(result) {
		if(messageNode) {
			*messageNode = FSend(FSend(context, Message), newWithReceiver:selector:arguments:, receiver, FSymbolCreateWithSubstring(source + index, length), NULL);
		}
		if(outLength) {
			*outLength = length;
		}
	}
	return result;
}


bool FParseArgument(FObject *context, const char *source, size_t index, size_t *outLength, FObject **argumentNode) {
	size_t totalLength = 0;
	FObject *receiver = NULL;
	bool result = FParseParenthesizedExpression(context, source, index, &totalLength, &receiver) || FParseNullaryMessage(context, NULL, source, index, &totalLength, &receiver);
	if(result) {
		size_t whitespaceLength = 0, messageLength = 0;
		while(
			(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
		&&	FParseNullaryMessage(context, receiver, source, index + totalLength + whitespaceLength, &messageLength, &receiver)
		) {
			totalLength += whitespaceLength + messageLength;
			
			whitespaceLength = messageLength = 0;
		}
		
		if(outLength) *outLength = totalLength;
		if(argumentNode) *argumentNode = receiver;
	}
	return result;
}

bool FParseKeywordArgument(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, size_t *outKeywordLength, FObject **argumentNode) {
	size_t keywordLength = 0, whitespaceLength = 0, argumentLength = 0;
	bool result =
		FParseKeyword(source, index, &keywordLength)
	&&	(FParseWhitespaceAndNewlines(source, index + keywordLength, &whitespaceLength) || 1)
	&&	FParseArgument(context, source, index + keywordLength + whitespaceLength, &argumentLength, argumentNode);
	if(outKeywordLength && result) *outKeywordLength = keywordLength;
	if(outLength && result) *outLength = keywordLength + whitespaceLength + argumentLength;
	return result;
}

bool FParseNAryMessage(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode) {
	size_t totalLength = 0, selectorLength = 0;
	char *selector = NULL;
	FObject
		*message = FSend(FSend(context, Message), newWithReceiver:selector:arguments:, receiver, NULL, NULL),
		*argument = NULL;
	FObject *arguments = NULL;
	size_t keywordLength = 0, keywordArgumentLength = 0, whitespaceLength = 0;
	unsigned count = 0;
	while(
		(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
	&&	FParseKeywordArgument(context, receiver, source, index + totalLength + whitespaceLength, &keywordArgumentLength, &keywordLength, &argument)
	) {
		size_t start = index + totalLength;
		totalLength += keywordArgumentLength + whitespaceLength;
		selectorLength += keywordLength;
		count++;
		
		// add to the selector
		selector = realloc(selector, selectorLength);
		memcpy(selector + selectorLength - keywordLength, source + start + whitespaceLength, keywordLength);
		
		// set the argument
		FObject *argumentNode = FSend(FSend(context, ListNode), newWithObject:, argument);
		arguments = (arguments == NULL)?
			FSend(message, arguments:, argumentNode)
		:	FSend(arguments, next:, argumentNode);
		
		keywordLength = keywordArgumentLength = whitespaceLength = 0;
	}
	bool result = (count > 0);
	if(result && outLength) *outLength = totalLength;
	if(result && messageNode) {
		FSend(message, selector:, (FObject *)FSymbolCreateWithSubstring(selector, selectorLength));
		*messageNode = message;
	}
	return result;
}

bool FParseMessage(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode) {
	return
		FParseNAryMessage(context, receiver, source, index, outLength, messageNode)
	||	FParseNullaryMessage(context, receiver, source, index, outLength, messageNode);
}


/*
	{ foo, bar -> foo or: bar }
	{ -> 1 }
	{ 2 }
	
	\ foo, bar -> foo or: bar
	\ -> 3
	\ 4
*/
bool FParseParameter(FObject *context, const char *source, size_t index, size_t *outLength, FObject **symbol) {
	size_t length = 0;
	if(FParseWord(source, index, &length)) {
		if(outLength) *outLength = length;
		if(symbol) *symbol = FSymbolCreateWithSubstring(source + index, length);
	}
	return length > 0;
}

bool FParseParameterList(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outParameterNode) {
	bool result = 1;
	size_t totalLength = 0;
	FObject *rootNode = NULL, *currentNode = NULL;
	do {
		FObject *parameter = NULL;
		size_t whitespaceLength = 0, parameterLength = 0;
		result =
			(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
		&&	FParseParameter(context, source, index + totalLength + whitespaceLength, &parameterLength, &parameter);
		
		if(result) {
			totalLength += whitespaceLength + parameterLength;
			if(outParameterNode) {
				FObject *node = FSend(FSend(context, ListNode), newWithObject:, parameter);
				if(currentNode) {
					FSend(currentNode, next:, node);
					currentNode = node;
				} else {
					rootNode = currentNode = node;
				}
			}
		}
	} while(result && FParseToken(source, index + totalLength, ",") && (totalLength += 1));
	
	if(result) {
		if(outLength) *outLength = totalLength;
		if(outParameterNode) *outParameterNode = rootNode;
	}
	return result;
}

bool FParseParameterListAndFunctionBodySeparator(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outParameterNode) {
	size_t parameterListLength = 0, whitespaceLength = 0;
	bool result =
		FParseParameterList(context, source, index, &parameterListLength, outParameterNode)
	&&	(FParseWhitespaceAndNewlines(source, index + parameterListLength, &whitespaceLength) || 1)
	&&	FParseToken(source, index + parameterListLength + whitespaceLength, "->");
	if(result) {
		if(outLength) *outLength = parameterListLength + whitespaceLength + 2;
	}
	return result;
}


bool FParseNAryFunction(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outFunction) {
	size_t openingWhitespaceLength = 0, closingWhitespaceLength = 0, parameterListLength = 0, expressionListLength = 0;
	FObject *parameterList = NULL, *expressionList = NULL;
	bool result =
		FParseToken(source, index, "{")
	&&	(FParseWhitespaceAndNewlines(source, index + 1, &openingWhitespaceLength) || 1)
	&&	FParseParameterListAndFunctionBodySeparator(context, source, index + 1 + openingWhitespaceLength, &parameterListLength, &parameterList)
	&&	FParseExpressionList(context, source, index + 1 + openingWhitespaceLength + parameterListLength, &expressionListLength, &expressionList)
	&&	(FParseWhitespaceAndNewlines(source, index + 1 + openingWhitespaceLength + parameterListLength + expressionListLength, &closingWhitespaceLength) || 1)
	&&	FParseToken(source, index + 1 + openingWhitespaceLength + parameterListLength + expressionListLength + closingWhitespaceLength, "}");
	if(result) {
		if(outLength) *outLength = index + 1 + openingWhitespaceLength + parameterListLength + expressionListLength + closingWhitespaceLength + 1;
		if(outFunction) *outFunction = FSend(FSend(context, Function), newWithContext:arguments:messages:, NULL, parameterList, expressionList);
	}
	return result;
}


bool FParseNullaryFunction(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outFunction) {
	size_t openingWhitespaceLength = 0, closingWhitespaceLength = 0, expressionListLength = 0;
	FObject *expressionList = NULL;
	bool result =
		FParseToken(source, index, "{")
	&&	(FParseWhitespaceAndNewlines(source, index + 1, &openingWhitespaceLength) || 1)
	&&	FParseExpressionList(context, source, index + 1 + openingWhitespaceLength, &expressionListLength, &expressionList)
	&&	(FParseWhitespaceAndNewlines(source, index + 1 + openingWhitespaceLength + expressionListLength, &closingWhitespaceLength) || 1)
	&&	FParseToken(source, index + 1 + openingWhitespaceLength + expressionListLength + closingWhitespaceLength, "}");
	if(result) {
		if(outLength) *outLength = index + 1 + openingWhitespaceLength + expressionListLength + closingWhitespaceLength + 1;
		if(outFunction) *outFunction = FSend(FSend(context, Function), newWithContext:arguments:messages:, NULL, NULL, expressionList);
	}
	return result;
}

bool FParseFunction(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outFunction) {
	return
		FParseNAryFunction(context, source, index, outLength, outFunction)
	||	FParseNullaryFunction(context, source, index, outLength, outFunction);
}


bool FParseExpression(FObject *context, const char *source, size_t index, size_t *outLength, FObject **expressionNode) {
	size_t totalLength = 0;
	FObject *receiver = NULL;
	bool result = FParseParenthesizedExpression(context, source, index, &totalLength, &receiver) || FParseMessage(context, NULL, source, index, &totalLength, &receiver);
	if(result) {
		size_t whitespaceLength = 0, messageLength = 0;
		while(
			(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
		&&	FParseMessage(context, receiver, source, index + totalLength + whitespaceLength, &messageLength, &receiver)
		) {
			totalLength += whitespaceLength + messageLength;
			whitespaceLength = messageLength = 0;
		}
		
		if(outLength) *outLength = totalLength;
		if(expressionNode) *expressionNode = receiver;
	}
	
	return result;
}

bool FParseParenthesizedExpression(FObject *context, const char *source, size_t index, size_t *outLength, FObject **expressionNode) {
	size_t expressionLength = 0, openingWhitespaceLength = 0, closingWhitespaceLength = 0;
	bool result =
		FParseToken(source, index, "(")
	&&	(FParseWhitespaceAndNewlines(source, index + 1, &openingWhitespaceLength) || 1)
	&&	FParseExpression(context, source, index + 1 + openingWhitespaceLength, &expressionLength, expressionNode)
	&&	(FParseWhitespaceAndNewlines(source, index + 1 + openingWhitespaceLength + expressionLength, &closingWhitespaceLength) || 1)
	&&	FParseToken(source, index + 1 + openingWhitespaceLength + expressionLength + closingWhitespaceLength, ")");
	if(outLength && result) *outLength = 1 + openingWhitespaceLength + expressionLength + closingWhitespaceLength + 1;
	return result;
}

bool FParseExpressionList(FObject *context, const char *source, size_t index, size_t *outLength, FObject **listNode) {
	bool result = 0;
	size_t whitespaceLength = 0, expressionLength = 0, totalLength = 0;
	FObject *rootNode = NULL, *currentNode = NULL, *expression = NULL;
	while(
		(FParseWhitespaceAndNewlines(source, index + totalLength, &whitespaceLength) || 1)
	&&	FParseExpression(context, source, index + totalLength + whitespaceLength, &expressionLength, &expression)
	) {
		totalLength += whitespaceLength + expressionLength;
		whitespaceLength = expressionLength = 0;
		result = 1;
		if(listNode) {
			FObject *node = FSend(FSend(context, ListNode), newWithObject:, expression);
			if(currentNode) {
				FSend(currentNode, next:, node);
				currentNode = node;
			} else {
				rootNode = currentNode = node;
			}
		}
	}
	if(result) {
		if(outLength) *outLength = totalLength;
		if(listNode) *listNode = rootNode;
	}
	return result;
}


bool FParse(FObject *context, const char *source, FObject **outFunction) {
	bool result = 0;
	FObject *expressionList = NULL;
	size_t expressionListLength = 0, whitespaceLength = 0, length = strlen(source);
	if(
		FParseExpressionList(context, source, 0, &expressionListLength, &expressionList)
	&&	(FParseWhitespaceAndNewlines(source, expressionListLength, &whitespaceLength) || 1)
	&&	(length == expressionListLength + whitespaceLength)
	) {
		if(outFunction) {
			*outFunction = FSend(FSend(context, Function), newWithContext:arguments:messages:, NULL, NULL, expressionList);
		}
		result = 1;
	}
	return result;
}
