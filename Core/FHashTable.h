// FHashTable.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_HASH_TABLE
#define F_HASH_TABLE

#include "FSymbol.h"

typedef struct FHashTable {
	unsigned int bucketCount;
	struct FPair *buckets;
} FHashTable;

FHashTable *FHashTableCreate();

void *FHashTableGetValueForKey(FHashTable *self, struct FSymbol *key);
void FHashTableSetValueForKey(FHashTable *self, struct FSymbol *key, void *value); // assumes you aren’t inserting a duplicate entry for the given key

#endif // F_HASH_TABLE
