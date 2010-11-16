// FSymbolTests.c
// Created by Rob Rix on 2010-09-29
// Copyright 2010 Monochrome Industries

#include "Core/FSymbol.h"
#include "FTestSuite.h"

static FSymbol *symbol = NULL;

static void setUp() {
	symbol = FSymbolCreateWithString("symbol:");
}


static void testCreation() {
	FAssert(symbol != NULL);
}

static void testCanBeCompared() {
	FSymbol *same = FSymbolCreateWithString("symbol:"), *different = FSymbolCreateWithString("fishstick");
	FAssert(FSymbolIsEqual(symbol, same));
	FAssert(!FSymbolIsEqual(symbol, different));
}

static void testHashesItsSymbol() {
	FSymbol *same = FSymbolCreateWithString("symbol:"), *different = FSymbolCreateWithString("fishstick");
	FAssert(FSymbolGetHash(symbol) != 0);
	FAssert(FSymbolGetHash(symbol) == FSymbolGetHash(same));
	FAssert(FSymbolGetHash(symbol) != FSymbolGetHash(different));
}


static void testCalculatesItsArity() {
	FAssert(FSymbolGetArity(FSymbolCreateWithString("nullary")) == 0);
	FAssert(FSymbolGetArity(FSymbolCreateWithString("unary:")) == 1);
	FAssert(FSymbolGetArity(FSymbolCreateWithString("bin:ary:")) == 2);
}


void FRunSymbolTests() {
	FRunTestSuite("FSymbol", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreation),
		FTestCase(testCanBeCompared),
		FTestCase(testHashesItsSymbol),
		
		FTestCase(testCalculatesItsArity),
		{0},
	});
}
