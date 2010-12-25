// FSymbol.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_SYMBOL
#define F_SYMBOL

#include "FObject.h"
#include <stdbool.h>
#include <string.h>

FObject *FSymbolCreateWithPrototypeAndString(FObject *prototype, const char *symbol);
FObject *FSymbolCreateWithPrototypeAndSubstring(FObject *prototype, const char *symbol, size_t length);
FObject *FSymbolCreateWithString(const char *symbol);
FObject *FSymbolCreateWithSubstring(const char *symbol, size_t length);

bool FSymbolIsEqual(FObject *a, FObject *b);

const char *FSymbolGetString(FObject *self);
unsigned long FSymbolGetHash(FObject *self);

size_t FSymbolGetArity(FObject *self);

#endif // F_SYMBOL
