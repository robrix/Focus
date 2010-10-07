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


bool FParseSelectorFragment(const char *source, size_t index, size_t *outLength) {
	size_t wordLength = 0;
	bool result =
		(FParseWord(source, index, &wordLength) || 1)
	&&	FParseToken(source, index + wordLength, ":");
	if(outLength && result) *outLength = wordLength + 1;
	return result;
}
