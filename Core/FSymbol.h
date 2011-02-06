// FSymbol.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_SYMBOL
#define F_SYMBOL

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifdef __LP64__
	typedef uint64_t FUInteger;
	typedef int64_t FInteger;
#else
	typedef uint32_t FUInteger;
	typedef int32_t FInteger;
#endif

typedef FUInteger FHash;

typedef struct FSymbol {
	const char *key;
	FHash hash;
} FSymbol;

void FSymbolInitialize();

FSymbol *FSymbolCreateWithString(const char *symbol);
FSymbol *FSymbolCreateWithSubstring(const char *symbol, size_t length);

bool FSymbolIsEqual(FSymbol *a, FSymbol *b);

const char *FSymbolGetString(FSymbol *self);
unsigned long FSymbolGetHash(FSymbol *self);

size_t FSymbolGetArity(FSymbol *self);

unsigned long FSymbolCalculateHashForString(const char *str);

#endif // F_SYMBOL
