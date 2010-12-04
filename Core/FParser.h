// FParser.h
// Created by Rob Rix on 2010-10-04
// Copyright 2010 Monochrome Industries

#ifndef F_PARSER
#define F_PARSER

#include <stdbool.h>
#include <stddef.h>

bool FParseCharacterSet(const char *source, size_t index, const char *characters, size_t *outLength);
bool FParseToken(const char *source, size_t index, const char *token);

bool FParseWord(const char *source, size_t index, size_t *outLength);
bool FParseWhitespace(const char *source, size_t index, size_t *outLength);
bool FParseWhitespaceAndNewlines(const char *source, size_t index, size_t *outLength);

bool FParseKeyword(const char *source, size_t index, size_t *outLength);

bool FParseNullaryMessage(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **messageNode);
bool FParseNAryMessage(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **messageNode);
bool FParseMessage(struct FObject *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **messageNode);

bool FParseParameter(const char *source, size_t index, size_t *outLength, struct FSymbol **symbol);
bool FParseParameterList(const char *source, size_t index, size_t *outLength, struct FObject **outParameterNode);

bool FParseNAryFunction(const char *source, size_t index, size_t *outLength, struct FObject **outFunction);
bool FParseNullaryFunction(const char *source, size_t index, size_t *outLength, struct FObject **outFunction);

bool FParseExpression(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **expressionNode);
bool FParseParenthesizedExpression(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **expressionNode);
bool FParseExpressionList(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FObject **listNode);

void *FParse(const char *source);

#endif // F_PARSER
