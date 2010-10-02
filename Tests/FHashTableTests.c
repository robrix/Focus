// FHashTableTests.c
// Created by Rob Rix on 2010-09-14
// Copyright 2010 Monochrome Industries

#include "Core/FHashTable.h"
#include "Core/FSymbol.h"
#include "FTestSuite.h"

static FHashTable *hashTable = NULL;

static void setUp() {
	hashTable = FHashTableCreate();
}


static void testCreation() {
	FAssert(hashTable != NULL);
}

static void testStoresObjectsByKeys() {
	FSymbol *key = FSymbolCreate("key");
	void *value = testCreation;
	FHashTableSetValueForKey(hashTable, key, value);
	void *result = FHashTableGetValueForKey(hashTable, key);
	FAssert(result == value);
}

void FRunHashTableTests() {
	FRunTestSuite("FHashTable", setUp, NULL, (FTestSuiteTestFunction[]){
		testCreation,
		testStoresObjectsByKeys,
		NULL,
	});
}
