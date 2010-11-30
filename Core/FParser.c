// FParser.c
// Created by Rob Rix on 2010-10-04
// Copyright 2010 Monochrome Industries

#include "Prototypes/FMessagePrototype.h"
#include "Prototypes/FListNodePrototype.h"
#include "FParser.h"
#include "FSymbol.h"
#include "FFunction.h"
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


bool FParseNullaryMessage(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **messageNode) {
	size_t length = 0;
	bool result = FParseWord(source, index, &length) && !FParseToken(source, index + length, ":");
	if(result) {
		if(messageNode) {
			*messageNode = FMessageCreateNullaryWithSubstring(context, receiver, source + index, length);
		}
		if(outLength) {
			*outLength = length;
		}
	}
	return result;
}


bool FParseArgument(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **argumentNode) {
	size_t totalLength = 0;
	FObject *receiver = NULL;
	bool result = FParseParenthesizedExpression(context, source, index, &totalLength, &receiver) || FParseNullaryMessage(NULL, context, source, index, &totalLength, &receiver);
	if(result) {
		size_t whitespaceLength = 0, messageLength = 0;
		while(
			(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
		&&	FParseNullaryMessage(receiver, context, source, index + totalLength + whitespaceLength, &messageLength, &receiver)
		) {
			totalLength += whitespaceLength + messageLength;
			
			whitespaceLength = messageLength = 0;
		}
		
		if(outLength) *outLength = totalLength;
		if(argumentNode) *argumentNode = receiver;
	}
	return result;
}

bool FParseKeywordArgument(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, size_t *outKeywordLength, struct FObject **argumentNode) {
	size_t keywordLength = 0, whitespaceLength = 0, argumentLength = 0;
	bool result =
		FParseKeyword(source, index, &keywordLength)
	&&	(FParseWhitespaceAndNewlines(source, index + keywordLength, &whitespaceLength) || 1)
	&&	FParseArgument(context, source, index + keywordLength + whitespaceLength, &argumentLength, argumentNode);
	if(outKeywordLength && result) *outKeywordLength = keywordLength;
	if(outLength && result) *outLength = keywordLength + whitespaceLength + argumentLength;
	return result;
}

bool FParseNAryMessage(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **messageNode) {
	size_t totalLength = 0, selectorLength = 0;
	char *selector = NULL;
	FObject
		*message = FMessageCreate(context, receiver, NULL, NULL),
		*argument = NULL;
	FObject *arguments = NULL;
	size_t keywordLength = 0, keywordArgumentLength = 0, whitespaceLength = 0;
	unsigned count = 0;
	while(
		(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
	&&	FParseKeywordArgument(receiver, context, source, index + totalLength + whitespaceLength, &keywordArgumentLength, &keywordLength, &argument)
	) {
		size_t start = index + totalLength;
		totalLength += keywordArgumentLength + whitespaceLength;
		selectorLength += keywordLength;
		count++;
		
		// add to the selector
		selector = realloc(selector, selectorLength);
		memcpy(selector + selectorLength - keywordLength, source + start + whitespaceLength, keywordLength);
		
		// set the argument
		if(arguments == NULL) {
			arguments = FSend(message, arguments:, FListNodeCreateWithObject(argument));
		} else {
			arguments = FSend(arguments, next:, FListNodeCreateWithObject(argument));
		}
		
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

bool FParseMessage(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **messageNode) {
	return
		FParseNAryMessage(receiver, context, source, index, outLength, messageNode)
	||	FParseNullaryMessage(receiver, context, source, index, outLength, messageNode);
}


bool FParseExpression(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **expressionNode) {
	size_t totalLength = 0;
	FObject *receiver = NULL;
	bool result = FParseParenthesizedExpression(context, source, index, &totalLength, &receiver) || FParseMessage(NULL, context, source, index, &totalLength, &receiver);
	if(result) {
		size_t whitespaceLength = 0, messageLength = 0;
		while(
			(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
		&&	FParseMessage(receiver, context, source, index + totalLength + whitespaceLength, &messageLength, &receiver)
		) {
			totalLength += whitespaceLength + messageLength;
			whitespaceLength = messageLength = 0;
		}
		
		if(outLength) *outLength = totalLength;
		if(expressionNode) *expressionNode = receiver;
	}
	
	return result;
}

bool FParseParenthesizedExpression(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **expressionNode) {
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
