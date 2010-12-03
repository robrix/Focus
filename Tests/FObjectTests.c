// FObjectTests.c
// Created by Rob Rix on 2010-10-02
// Copyright 2010 Monochrome Industries

#include "Core/FObject.h"
#include "Core/FSymbol.h"
#include "Core/FFunction.h"
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

static void testInheritsMethodsFromItsPrototype() {
	FObjectSetMethod(object, FSymbolCreateWithString("method"), FFunctionCreateWithImplementation(NULL, (FImplementation)method));
	FObject *clone = FObjectCreate(object);
	FAssert(FFunctionGetImplementation(FObjectGetMethod(clone, FSymbolCreateWithString("method"))) == (FImplementation)method);
	
	FAssert(FSend(clone, method) == NULL);
}


void FRunObjectTests() {
	FRunTestSuite("FObject", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreation),
		FTestCase(testInheritsMethodsFromItsPrototype),
		{0},
	});
}
