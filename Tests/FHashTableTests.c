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
	FSymbol *key = FSymbolCreateWithString("key");
	void *value = testCreation;
	FHashTableSetValueForKey(hashTable, key, value);
	void *result = FHashTableGetValueForKey(hashTable, key);
	FAssert(result == value);
}


static void FHashTableTestsCountSlots(FHashTable *table, FSlot *slot, void *context) {
	*(uint16_t *)context = (*(uint16_t *)context) + 1;
}

static void testVisitsSlotsWithVisitorFunction() {
	uint16_t count = 0;
	FHashTableVisitSlots(hashTable, FHashTableTestsCountSlots, &count);
	FAssert(count == 0);
	
	// add stuff and test again
	FHashTableSetValueForKey(hashTable, FSymbolCreateWithString("key"), NULL);
	FHashTableVisitSlots(hashTable, FHashTableTestsCountSlots, &count);
	FAssert(count == 1);
}

void FRunHashTableTests() {
	FRunTestSuite(&(FTestSuite){"FHashTable", setUp, NULL, (FTestCase[]){
		FTestCase(testCreation),
		FTestCase(testStoresObjectsByKeys),
		FTestCase(testVisitsSlotsWithVisitorFunction),
		{0},
	}});
}
