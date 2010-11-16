// FSymbol.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_SYMBOL
#define F_SYMBOL

#include "FObject.h"
#include <stdbool.h>
#include <string.h>

typedef struct FSymbol FSymbol;

FSymbol *FSymbolCreateWithString(const char *symbol);
FSymbol *FSymbolCreateWithSubstring(const char *symbol, size_t length);

bool FSymbolIsEqual(FSymbol *a, FSymbol *b);

const char *FSymbolGetString(FSymbol *self);
unsigned long FSymbolGetHash(FSymbol *self);

size_t FSymbolGetArity(FSymbol *self);

#endif // F_SYMBOL
