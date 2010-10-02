// FSymbol.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_SYMBOL
#define F_SYMBOL

#include "FObject.h"
#include <stdbool.h>

typedef struct FSymbol {
	FObject super;
	unsigned long hash;
	const char *symbol;
} FSymbol;

FSymbol *FSymbolCreate(const char *symbol);

bool FSymbolIsEqual(FSymbol *a, FSymbol *b);

#endif // F_SYMBOL
