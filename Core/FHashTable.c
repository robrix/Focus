// FHashTable.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FHashTable.h"
#include "FSymbol.h"
#include <stdlib.h>

FHashTable *FHashTableCreate() {
	
	return NULL;
}

void *FHashTableGetValueForKey(FHashTable *self, FSymbol *key) {
	unsigned int hash = key->hash; // get the symbol’s hash
	FPair *pair = &self->buckets[hash % self->bucketCount];
	void *value = NULL;
	do {
		if(FSymbolIsEqual(pair->key, key)) {
			value = pair->value;
			break;
		}
	} while((pair = pair->nextPair));
	return value;
}

void FHashTableSetValueForKey(FHashTable *self, FSymbol *key, void *value) {
	
}
