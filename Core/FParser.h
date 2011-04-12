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

bool FParseNullaryMessage(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode);
bool FParseNAryMessage(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode);
bool FParseMessage(FObject *context, FObject *receiver, const char *source, size_t index, size_t *outLength, FObject **messageNode);

bool FParseParameter(FObject *context, const char *source, size_t index, size_t *outLength, FSymbol **symbol);
bool FParseParameterList(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outParameterNode);

bool FParseNAryFunction(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outFunction);
bool FParseNullaryFunction(FObject *context, const char *source, size_t index, size_t *outLength, FObject **outFunction);

bool FParseExpression(FObject *context, const char *source, size_t index, size_t *outLength, FObject **expressionNode);
bool FParseParenthesizedExpression(FObject *context, const char *source, size_t index, size_t *outLength, FObject **expressionNode);
bool FParseExpressionList(FObject *context, const char *source, size_t index, size_t *outLength, FObject **listNode);

bool FParse(FObject *context, const char *source, FObject **outFunction);

#endif // F_PARSER
