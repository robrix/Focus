// FObjectTests.c
// Created by Rob Rix on 2010-10-02
// Copyright 2010 Monochrome Industries

#include "Core/FObject+Protected.h"
#include "Core/FSymbol.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "FTestSuite.h"

static FObject *object = NULL;

static void setUp() {
	FSetUpTestEvaluator();
	object = FObjectCreate(NULL);
}


static void testCreation() {
	FAssert(object != NULL);
}


static FObject *method(FObject *receiver, FSymbol *selector) {
	return NULL;
}

static void testInheritsMethodsFromItsPrototype() {
	FObjectSetMethod(object, FSymbolCreateWithString("method"), FFunctionCreateWithImplementation(FSend(FSend(FTestEvaluator, Context), Function), (FImplementation)method));
	FObject *clone = FObjectCreate(object);
	FAssert(FFunctionGetImplementation(FObjectGetMethod(clone, FSymbolCreateWithString("method"))) == (FImplementation)method);
	
	FAssert(FSend(clone, method) == NULL);
}


void FRunObjectTests() {
	FRunTestSuite(&(FTestSuite){"FObject", setUp, FTearDownTestEvaluator, (FTestCase[]){
		FTestCase(testCreation),
		FTestCase(testInheritsMethodsFromItsPrototype),
		{0},
	}});
}
