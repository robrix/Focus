// FSymbol.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FAssertions.h"
// #include "FEvaluator.h"
#include "FSymbol.h"
#include "FObject+Protected.h"
// #include "FHashTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const FSymbol F_SymbolSymbol = (FSymbol){ " symbol", (FUInteger)&F_SymbolSymbol };
const FSymbol F_HashSymbol = (FSymbol){ " hash", (FUInteger)&F_HashSymbol };

static FHashTable *FSymbolTable = NULL;

void FSymbolInitialize() {
	if(!FSymbolTable) {
		FSymbolTable = FHashTableCreate();
	}
}


// registers the symbol if it wasn’t already
FSymbol *FSymbolGetRegisteredSymbolWithStringAndHash(const char *string, FHash hash) {
	FAssertPrecondition(string != NULL);
	if(!FSymbolTable) FSymbolInitialize();
	FSymbol symbol = { .key = string, .hash = hash };
	FSymbol *registeredSymbol = FHashTableGetValueForKey(FSymbolTable, &symbol);
	if(!registeredSymbol) {
		registeredSymbol = FAllocatorAllocate(NULL, sizeof(FSymbol));
		*registeredSymbol = symbol;
		FHashTableSetValueForKey(FSymbolTable, registeredSymbol, registeredSymbol);
	}
	return registeredSymbol;
}


FSymbol *FSymbolCreateWithString(const char *symbol) {
	return FSymbolGetRegisteredSymbolWithStringAndHash(symbol, FSymbolCalculateHashForString(symbol));
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
	&&	(FSymbolGetHash(a) == FSymbolGetHash(b))
	&&	(strcmp(FSymbolGetString(a), FSymbolGetString(b)) == 0)
	);
}


const char *FSymbolGetString(FSymbol *self) {
	return self->key;
}

unsigned long FSymbolGetHash(FSymbol *self) {
	return self->hash;
}


size_t FSymbolGetArity(FSymbol *self) {
	size_t count = 0;
	char *symbol = (char *)FSymbolGetString(self);
	while((symbol = index(symbol + 1, ':'))) {
		count++;
	}
	return count;
}


unsigned long FSymbolCalculateHashForString(const char *str) {
	unsigned long hash = 5381;
	int c;
	
	while((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}
