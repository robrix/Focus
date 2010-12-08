// FHashTable.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FHashTable.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct FPair {
	struct FObject *key;
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

FPair *FHashTableGetPairForKeyWithHash(FHashTable *self, FObject *key, size_t hash) {
	if(!key) {
		printf("null key!\n");
		fflush(stdout);
	}
	FPair *pair = FHashTableGetBucketForHash(self, hash);
	while((pair->key != NULL) && (pair != NULL)) {
		if(FSymbolIsEqual(pair->key, key)) {
			break;
		} else {
			pair = pair->nextPair;
		}
	}
	return pair;
}

// FPair *FHashTableGetPairForKeyWithHash(FHashTable *self, FObject *key, size_t hash) {
// 	
// }

void *FHashTableGetValueForKey(FHashTable *self, FObject *key) {
	return FHashTableGetValueForKeyWithHash(self, key, FSymbolGetHash(key));
}

void *FHashTableGetValueForKeyWithHash(FHashTable *self, FObject *key, size_t hash) {
	if(!key) {
		printf("null key!\n");
		fflush(stdout);
	}
	FPair *pair = FHashTableGetPairForKeyWithHash(self, key, hash);
	return (pair != NULL)
	?	pair->value
	:	NULL;
}

void FHashTableSetValueForKey(FHashTable *self, FObject *key, void *value) {
	FHashTableSetValueForKeyWithHash(self, key, FSymbolGetHash(key), value);
}

void FHashTableSetValueForKeyWithHash(FHashTable *self, FObject *key, size_t hash, void *value) {
	if(!key) {
		printf("null key!\n");
		fflush(stdout);
		abort();
	}
	if(!value) {
		// fixme: this should be a debug log
		printf("null value!\n");
		fflush(stdout);
	}
	FPair *pair = FPairGetTail(FHashTableGetBucketForHash(self, hash));
	pair->key = key;
	pair->value = value;
	pair->nextPair = FAllocatorAllocate(NULL, sizeof(FPair));
}
