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

bool FParseNullaryMessage(struct FMessage *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FMessage **messageNode);
bool FParseNAryMessage(struct FMessage *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FMessage **messageNode);
bool FParseMessage(struct FMessage *receiver, struct FObject *context, const char *source, size_t index, size_t *outLength, struct FMessage **messageNode);

bool FParseExpression(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FMessage **expressionNode);
bool FParseParenthesizedExpression(struct FObject *context, const char *source, size_t index, size_t *outLength, struct FMessage **expressionNode);

void *FParse(const char *source);

#endif // F_PARSER
