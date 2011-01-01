// FSymbol.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FEvaluator.h"
#include "FSymbol.h"
#include "FHashTable.h"
#include "FObject+Protected.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// fixme: unique symbols
// FHashTable *FSymbolGetSymbolTable() {
// 	static FHashTable *table = NULL;
// 	if(!table) {
// 		table = FHashTableCreate();
// 		// FObject *_symbol = FAllocatorAllocate(NULL, sizeof(FObject));
// 		// FHashTableSetValueForKey(table, _symbol, _symbol);
// 		// FObjectSetVariable(_symbol, FSymbolCreateWithString(" symbol"), _symbol);
// 	}
// 	return table;
// }

unsigned long FSymbolCalculateHashForString(const char *str) {
	unsigned long hash = 5381;
	int c;
	
	while((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}


FObject *FSymbolGetHashSymbolWithPrototype(FObject *prototype);

// :)
FObject *FSymbolGetSymbolSymbolWithPrototype(FObject *prototype) {
	FObject *symbol = FObjectCreate(prototype);
	size_t symbolHash = FSymbolCalculateHashForString(" symbol");
	FObjectSetVariableWithHash(symbol, FSymbolGetSymbolSymbolWithPrototype(prototype), symbolHash, (FObject *)" symbol");
	FObjectSetVariableWithHash(symbol, FSymbolGetHashSymbolWithPrototype(prototype), FSymbolCalculateHashForString(" hash"), (FObject *)symbolHash);
	return symbol;
}

FObject *FSymbolGetHashSymbolWithPrototype(FObject *prototype) {
	FObject *hash = FObjectCreate(prototype);
	size_t hashHash = FSymbolCalculateHashForString(" hash"); // :)
	FObjectSetVariableWithHash(hash, FSymbolGetSymbolSymbolWithPrototype(prototype), FSymbolCalculateHashForString(" symbol"), (FObject *)" hash");
	FObjectSetVariableWithHash(hash, FSymbolGetHashSymbolWithPrototype(prototype), hashHash, (FObject *)hashHash);
	return hash;
}


FObject *FSymbolCreateWithString(FObject *context, const char *symbol) {
	// FHashTableGetValueForKey(FSymbolTable(), )
	FObject *prototype = FSend(context, Symbol);
	FObject *instance = FObjectCreate(prototype);
	FObjectSetVariableWithHash(instance, FSymbolGetSymbolSymbolWithPrototype(prototype), FSymbolCalculateHashForString(" symbol"), (FObject *)symbol);
	FObjectSetVariableWithHash(instance, FSymbolGetHashSymbolWithPrototype(prototype), FSymbolCalculateHashForString(" hash"), (FObject *)FSymbolCalculateHashForString(symbol));
	return instance;
}

FObject *FSymbolCreateWithSubstring(FObject *context, const char *symbol, size_t length) {
	return FSymbolCreateWithString(context, strncpy(FAllocatorAllocate(NULL, length), symbol, length));
}


bool FSymbolIsEqual(FObject *a, FObject *b) {
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


const char *FSymbolGetString(FObject *self) {
	// fixme: cache the hash
	return (const char *)FObjectGetVariableWithHash(self, FSymbolGetSymbolSymbolWithPrototype(FObjectGetPrototype(self)), FSymbolCalculateHashForString(" symbol"));
}

unsigned long FSymbolGetHash(FObject *self) {
	// fixme: cache the hash
	return (unsigned long)FObjectGetVariableWithHash(self, FSymbolGetHashSymbolWithPrototype(FObjectGetPrototype(self)), FSymbolCalculateHashForString(" hash"));
}


size_t FSymbolGetArity(FObject *self) {
	size_t count = 0;
	char *symbol = (char *)FSymbolGetString(self);
	while((symbol = index(symbol + 1, ':'))) {
		count++;
	}
	return count;
}
