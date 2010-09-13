// FHashTable.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_HASH_TABLE
#define F_HASH_TABLE

typedef struct FPair {
	void *key;
	void *value;
	struct FPair *nextPair;
} FPair;

typedef struct FHashTable {
	unsigned int bucketCount;
	FPair *buckets;
} FHashTable;

FHashTable *FHashTableCreate();

void *FHashTableGetValueForKey(FHashTable *self, void *key);
void FHashTableSetValueForKey(FHashTable *self, void *key, void *value);

#endif // F_HASH_TABLE
