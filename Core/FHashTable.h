// FHashTable.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_HASH_TABLE
#define F_HASH_TABLE

#include "FObject.h"

typedef struct FHashTable {
	unsigned int bucketCount;
	struct FPair *buckets;
} FHashTable;

FHashTable *FHashTableCreate();

void *FHashTableGetValueForKey(FHashTable *self, FObject *key);
void *FHashTableGetValueForKeyWithHash(FHashTable *self, FObject *key, size_t hash); // for bootstrapping symbols
void FHashTableSetValueForKey(FHashTable *self, FObject *key, void *value); // fixme: assumes you aren’t inserting a duplicate entry for the given key
void FHashTableSetValueForKeyWithHash(FHashTable *self, FObject *key, size_t hash, void *value); // for bootstrapping symbols (themselves a kind of object) as keys

#endif // F_HASH_TABLE
