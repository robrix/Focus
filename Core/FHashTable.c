// FHashTable.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FHashTable.h"
#include <stdlib.h>

typedef struct FPair {
	struct FSymbol *key;
	void *value;
	struct FPair *nextPair;
} FPair;


FHashTable *FHashTableCreate() {
	FHashTable *instance = FAllocatorAllocate(NULL, sizeof(FHashTable));
	instance->bucketCount = 19;
	instance->buckets = FAllocatorAllocate(NULL, instance->bucketCount * sizeof(FPair));
	// for(unsigned int i = 0; i < instance->bucketCount; i++) {
	// 	instance->buckets[i] = (FPair){ NULL, NULL, NULL };
	// }
	return instance;
}

FPair *FHashTableGetBucketForHash(FHashTable *self, unsigned int hash) {
	return (self->buckets + (hash % self->bucketCount));
}

FPair *FPairGetTail(FPair *pair) {
	while(pair->nextPair != NULL) {
		pair = pair->nextPair;
	}
	return pair;
}

FPair *FHashTableGetPairForKey(FHashTable *self, FSymbol *key) {
	FPair *pair = FHashTableGetBucketForHash(self, FSymbolGetHash(key));
	while(pair != NULL) {
		if(FSymbolIsEqual(pair->key, key)) {
			break;
		} else {
			pair = pair->nextPair;
		}
	}
	return pair;
}

void *FHashTableGetValueForKey(FHashTable *self, FSymbol *key) {
	FPair *pair = FHashTableGetPairForKey(self, key);
	return (pair != NULL)
	?	pair->value
	:	NULL;
}

void FHashTableSetValueForKey(FHashTable *self, FSymbol *key, void *value) {
	FPair *pair = FPairGetTail(FHashTableGetBucketForHash(self, FSymbolGetHash(key)));
	pair->key = key;
	pair->value = value;
	pair->nextPair = FAllocatorAllocate(NULL, sizeof(FPair));
}
