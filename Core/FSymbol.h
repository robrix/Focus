// FSymbol.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_SYMBOL
#define F_SYMBOL

#include "FObject.h"

typedef struct FSymbol {
	FObject super;
	unsigned int hash;
	const char *symbol;
} FSymbol;

FSymbol *FSymbolCreate();

#endif // F_SYMBOL
