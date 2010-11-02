// FParser.c
// Created by Rob Rix on 2010-10-04
// Copyright 2010 Monochrome Industries

#include "FMessage.h"
#include "FParser.h"
#include <ctype.h>
#include <string.h>

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


#warning fixme: messages should take a receiver node and a context object
bool FParseNullaryMessage(const char *source, size_t index, size_t *outLength, struct FMessage **messageNode) {
	size_t length = 0;
	bool result = FParseWord(source, index, &length);
	if(result) {
		if(messageNode) {
			*messageNode = FMessageCreateNullaryWithSubstring(NULL, NULL, source + index, length);
		}
		if(outLength) {
			*outLength = length;
		}
	}
	return result;
}

bool FParseArgument(const char *source, size_t index, size_t *outLength, struct FMessage **argumentNode) {
	return
		FParseParenthesizedExpression(source, index, outLength, argumentNode)
	||	FParseNullaryMessage(source, index, outLength, argumentNode);
}

bool FParseKeywordArgument(const char *source, size_t index, size_t *outLength, size_t *outKeywordLength, struct FMessage **argumentNode) {
	size_t keywordLength = 0, whitespaceLength = 0, argumentLength = 0;
	bool result =
		FParseKeyword(source, index, &keywordLength)
	&&	(FParseWhitespaceAndNewlines(source, index + keywordLength, &whitespaceLength) || 1)
	&&	FParseArgument(source, index + keywordLength + whitespaceLength, &argumentLength, argumentNode);
	if(outKeywordLength && result) *outKeywordLength = keywordLength;
	if(outLength && result) *outLength = keywordLength + whitespaceLength + argumentLength;
	return result;
}

bool FParseNAryMessage(const char *source, size_t index, size_t *outLength, struct FMessage **messageNode) {
	size_t totalLength = 0;
	struct FMessage *argument = NULL;
	bool result = 0;
	do {
		size_t keywordLength = 0, keywordArgumentLength = 0, whitespaceLength = 0;
		result =
			FParseKeywordArgument(source, index + totalLength, &keywordArgumentLength, &keywordLength, &argument)
		&&	(FParseWhitespaceAndNewlines(source, index + totalLength + keywordArgumentLength, &whitespaceLength) || 1);
		totalLength += keywordArgumentLength + whitespaceLength;
	} while(result == 1);
	if(outLength && totalLength) *outLength = totalLength;
	return (totalLength > 0);
}

bool FParseMessage(const char *source, size_t index, size_t *outLength, struct FMessage **messageNode) {
	return
		FParseNAryMessage(source, index, outLength, messageNode)
	||	FParseNullaryMessage(source, index, outLength, messageNode);
}


bool FParseExpression(const char *source, size_t index, size_t *outLength, struct FMessage **expressionNode) {
	size_t totalLength = 0;
	struct FMessage *receiver = NULL;
	bool
		result = FParseParenthesizedExpression(source, index, &totalLength, &receiver) || FParseMessage(source, index, &totalLength, &receiver),
		chain = 0;
	
	do {
		size_t whitespaceLength = 0, messageLength = 0;
		chain =
			(FParseWhitespace(source, index + totalLength, &whitespaceLength) || 1)
		&&	FParseMessage(source, index + totalLength + whitespaceLength, &messageLength, &receiver);
		if(chain) {
			totalLength += whitespaceLength + messageLength;
		}
	} while(chain == 1);
	
	if(result) {
		if(outLength) *outLength = totalLength;
		if(expressionNode) *expressionNode = receiver;
	}
	return result;
}

bool FParseParenthesizedExpression(const char *source, size_t index, size_t *outLength, struct FMessage **expressionNode) {
	size_t expressionLength = 0, openingWhitespaceLength = 0, closingWhitespaceLength = 0;
	bool result =
		FParseToken(source, index, "(")
	&&	(FParseWhitespaceAndNewlines(source, index + 1, &openingWhitespaceLength) || 1)
	&&	FParseExpression(source, index + 1 + openingWhitespaceLength, &expressionLength, expressionNode)
	&&	(FParseWhitespaceAndNewlines(source, index + 1 + openingWhitespaceLength + expressionLength, &closingWhitespaceLength) || 1)
	&&	FParseToken(source, index + 1 + openingWhitespaceLength + expressionLength + closingWhitespaceLength, ")");
	if(outLength && result) *outLength = 1 + openingWhitespaceLength + expressionLength + closingWhitespaceLength + 1;
	return result;
}