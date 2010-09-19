// FHashTableTests.c
// Created by Rob Rix on 2010-09-14
// Copyright 2010 Monochrome Industries

#include "Core/FHashTable.h"
#include "FTestSuite.h"
#include <stdio.h>

static FHashTable *hashTable = NULL;

static void setUp() {
	hashTable = FHashTableCreate();
}


static void testCreation() {
	FAssert(hashTable != NULL);
}

static void testStoresObjectsByKeys() {
	
}

__attribute__((constructor))
static void FTestHashTable() {
	FRunTestSuite("FHashTable", setUp, NULL, (FTestSuiteTestFunction *){
		testCreation,
		testStoresObjectsByKeys,
		NULL,
	});
}
