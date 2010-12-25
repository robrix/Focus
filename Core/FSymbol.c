// FSymbol.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
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


FObject *FSymbolGetHashSymbol();

// :)
FObject *FSymbolGetSymbolSymbolWithPrototype(FObject *prototype) {
	static FObject *_symbol = NULL;
	if(!_symbol) {
		_symbol = FObjectCreate(prototype);
		size_t _symbolHash = FSymbolCalculateHashForString(" symbol");
		FObjectSetVariableWithHash(_symbol, FSymbolGetSymbolSymbol(), _symbolHash, (FObject *)" symbol");
		FObjectSetVariableWithHash(_symbol, FSymbolGetHashSymbol(), FSymbolCalculateHashForString(" hash"), (FObject *)_symbolHash);
	}
	return _symbol;
}

FObject *FSymbolGetHashSymbolWithPrototype(FObject *prototype) {
	static FObject *_hash = NULL;
	if(!_hash) {
		_hash = FObjectCreate(prototype);
		size_t _hashHash = FSymbolCalculateHashForString(" hash"); // :)
		FObjectSetVariableWithHash(_hash, FSymbolGetSymbolSymbol(), FSymbolCalculateHashForString(" symbol"), (FObject *)" hash");
		FObjectSetVariableWithHash(_hash, FSymbolGetHashSymbol(), _hashHash, (FObject *)_hashHash);
		// FSymbolCreateWithString(" symbol");
	}
	return _hash;
}


FObject *FSymbolCreateWithPrototypeAndString(FObject *prototype, const char *symbol) {
	// FHashTableGetValueForKey(FSymbolTable(), )
	FObject *instance = FObjectCreate(prototype);
	FObjectSetVariableWithHash(instance, FSymbolGetSymbolSymbolWithPrototype(), FSymbolCalculateHashForString(" symbol"), (FObject *)symbol);
	FObjectSetVariableWithHash(instance, FSymbolGetHashSymbolWithPrototype(), FSymbolCalculateHashForString(" hash"), (FObject *)FSymbolCalculateHashForString(symbol));
	return instance;
}

FObject *FSymbolCreateWithPrototypeAndSubstring(FObject *prototype, const char *symbol, size_t length) {
	return FSymbolCreateWithPrototypeAndString(prototype, strncpy(FAllocatorAllocate(NULL, length), symbol, length));
}

#pragma message fixme: this has to go away
FObject *FSymbolCreateWithString(const char *symbol) {
	return FSymbolCreateWithPrototypeAndString(NULL, symbol);
}

FObject *FSymbolCreateWithSubstring(const char *symbol, size_t length) {
	return FSymbolCreateWithString(strncpy(FAllocatorAllocate(NULL, length), symbol, length));
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
	return (const char *)FObjectGetVariableWithHash(self, FSymbolGetSymbolSymbol(), FSymbolCalculateHashForString(" symbol"));
}

unsigned long FSymbolGetHash(FObject *self) {
	// fixme: cache the hash
	return (unsigned long)FObjectGetVariableWithHash(self, FSymbolGetHashSymbol(), FSymbolCalculateHashForString(" hash"));
}


size_t FSymbolGetArity(FObject *self) {
	size_t count = 0;
	char *symbol = (char *)FSymbolGetString(self);
	while((symbol = index(symbol + 1, ':'))) {
		count++;
	}
	return count;
}
