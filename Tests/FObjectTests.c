// FObjectTests.c
// Created by Rob Rix on 2010-10-02
// Copyright 2010 Monochrome Industries

#include "Core/FObject.h"
#include "Core/FSymbol.h"
#include "FTestSuite.h"

static FObject *object = NULL;

static void setUp() {
	object = FObjectCreate(NULL);
}


static void testCreation() {
	FAssert(object != NULL);
}


static FObject *method(FObject *receiver, FSymbol *selector) {
	return NULL;
}

static void testInheritance() {
	FObjectSetMethod(object, FSymbolCreateWithString("method"), (FMethod)method);
	FObject *clone = FObjectCreate(object);
	FAssert(FObjectGetMethod(clone, FSymbolCreateWithString("method")) == (FMethod)method);
}


void FRunObjectTests() {
	FRunTestSuite("FObject", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreation),
		FTestCase(testInheritance),
		{0},
	});
}
