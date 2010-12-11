// FParser.h
// Created by Rob Rix on 2010-10-04
// Copyright 2010 Monochrome Industries

#ifndef F_PARSER
#define F_PARSER

#include <stdbool.h>
#include <stddef.h>

#include "FObject.h"

bool FParseCharacterSet(const char *source, size_t index, const char *characters, size_t *outLength);
bool FParseToken(const char *source, size_t index, const char *token);

bool FParseWord(const char *source, size_t index, size_t *outLength);
bool FParseWhitespace(const char *source, size_t index, size_t *outLength);
bool FParseWhitespaceAndNewlines(const char *source, size_t index, size_t *outLength);

bool FParseKeyword(const char *source, size_t index, size_t *outLength);

bool FParseNullaryMessage(FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode);
bool FParseNAryMessage(FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode);
bool FParseMessage(FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode);

bool FParseParameter(const char *source, size_t index, size_t *outLength, FObject **symbol);
bool FParseParameterList(const char *source, size_t index, size_t *outLength, FObject **outParameterNode);

bool FParseNAryFunction(const char *source, size_t index, size_t *outLength, FObject **outFunction);
bool FParseNullaryFunction(const char *source, size_t index, size_t *outLength, FObject **outFunction);

bool FParseExpression(const char *source, size_t index, size_t *outLength, FObject **expressionNode);
bool FParseParenthesizedExpression(const char *source, size_t index, size_t *outLength, FObject **expressionNode);
bool FParseExpressionList(const char *source, size_t index, size_t *outLength, FObject **listNode);

FObject *FParse(const char *source); // returns a function

#endif // F_PARSER
