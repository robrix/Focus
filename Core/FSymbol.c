// FSymbol.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include <stdlib.h>
#include <string.h>

unsigned long FSymbolCalculateHashForString(const char *str) {
	unsigned long hash = 5381;
	int c;
	
	while((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}


FSymbol *FSymbolCreateWithString(const char *symbol) {
	FSymbol *instance = FAllocatorAllocate(NULL, sizeof(FSymbol));
	instance->symbol = symbol;
	instance->hash = FSymbolCalculateHashForString(instance->symbol);
	return instance;
}

FSymbol *FSymbolCreateWithSubstring(const char *symbol, size_t length) {
	return FSymbolCreateWithString(strncpy(FAllocatorAllocate(NULL, length), symbol, length));
}


bool FSymbolIsEqual(FSymbol *a, FSymbol *b) {
	return
		((a == b) && (a != NULL))
	||	(
		(a->hash == b->hash)
	&&	(strcmp(a->symbol, b->symbol) == 0)
	);
}
