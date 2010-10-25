// FParser.c
// Created by Rob Rix on 2010-10-04
// Copyright 2010 Monochrome Industries

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


bool FParseNullaryMessage(const char *source, size_t index, size_t *outLength, void **messageNode) {
	return FParseWord(source, index, outLength);
}

bool FParseKeywordArgument(const char *source, size_t index, size_t *outLength, size_t *outKeywordLength, void **argumentNode) {
	size_t keywordLength = 0, whitespaceLength = 0, argumentLength = 0;
	bool result =
		FParseKeyword(source, index, &keywordLength)
	&&	(FParseWhitespaceAndNewlines(source, index + keywordLength, &whitespaceLength) || 1)
	&&	FParseExpression(source, index + keywordLength + whitespaceLength, &argumentLength, argumentNode);
	if(outKeywordLength && result) *outKeywordLength = keywordLength;
	if(outLength && result) *outLength = keywordLength + whitespaceLength + argumentLength;
	return result;
}

bool FParseNAryMessage(const char *source, size_t index, size_t *outLength, void **messageNode) {
	size_t totalLength = 0;
	void *argument = NULL;
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

bool FParseMessage(const char *source, size_t index, size_t *outLength, void **messageNode) {
	return
		FParseNAryMessage(source, index, outLength, messageNode)
	||	FParseNullaryMessage(source, index, outLength, messageNode)
	;
}


bool FParseExpression(const char *source, size_t index, size_t *outLength, void **expressionNode) {
	return FParseParenthesizedExpression(source, index, outLength, expressionNode) || FParseMessage(source, index, outLength, expressionNode);
}

bool FParseParenthesizedExpression(const char *source, size_t index, size_t *outLength, void **expressionNode) {
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
