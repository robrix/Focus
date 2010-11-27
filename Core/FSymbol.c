// FSymbol.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FObject+Protected.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct FSymbol {
	FObject super;
	unsigned long hash;
	const char *symbol;
};


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
	// set the prototype
	return instance;
}

FSymbol *FSymbolCreateWithSubstring(const char *symbol, size_t length) {
	return FSymbolCreateWithString(strncpy(FAllocatorAllocate(NULL, length), symbol, length));
}


bool FSymbolIsEqual(FSymbol *a, FSymbol *b) {
	if(!a) {
		printf("a is null!\n");
		fflush(stdout);
	}
	if(!b) {
		printf("b is null!\n");
		fflush(stdout);
	}
	return
		((a == b) && (a != NULL))
	||	(
		((a != NULL) && (b != NULL))
	&&	(a->hash == b->hash)
	&&	(strcmp(a->symbol, b->symbol) == 0)
	);
}


const char *FSymbolGetString(FSymbol *self) {
	return self->symbol;
}

unsigned long FSymbolGetHash(FSymbol *self) {
	return self->hash;
}


size_t FSymbolGetArity(FSymbol *self) {
	size_t count = 0;
	char *symbol = (char *)self->symbol;
	while((symbol = index(symbol + 1, ':'))) {
		count++;
	}
	return count;
}
