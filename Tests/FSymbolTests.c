// FSymbolTests.c
// Created by Rob Rix on 2010-09-29
// Copyright 2010 Monochrome Industries

#include "Core/FSymbol.h"
#include "FTestSuite.h"

static FSymbol *symbol = NULL;

static void setUp() {
	symbol = FSymbolCreate("symbol:");
}


static void testCreation() {
	FAssert(symbol != NULL);
}

static void testCanBeCompared() {
	FSymbol *same = FSymbolCreate("symbol:"), *different = FSymbolCreate("fishstick");
	FAssert(FSymbolIsEqual(symbol, same));
	FAssert(!FSymbolIsEqual(symbol, different));
}

static void testHashesItsSymbol() {
	FSymbol *same = FSymbolCreate("symbol:"), *different = FSymbolCreate("fishstick");
	FAssert(symbol->hash != 0);
	FAssert(symbol->hash == same->hash);
	FAssert(symbol->hash != different->hash);
}

void FRunSymbolTests() {
	FRunTestSuite("FSymbol", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreation),
		FTestCase(testCanBeCompared),
		FTestCase(testHashesItsSymbol),
		{0},
	});
}
