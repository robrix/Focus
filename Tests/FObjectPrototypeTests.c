// FObjectPrototypeTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "FTestSuite.h"

static FObject *Object = NULL;
static void setUp() {
	Object = FSend(FSend(FTestEvaluator, Context), Object);
}

static void testCreatesASingletonPrototype() {
	FAssert(Object != NULL);
}

static void testImplementsTheIdentityMethod() {
	FAssert(Object == FSend(Object, self));
}

static void testCanBeCloned() {
	FAssert(FObjectGetPrototype(FSend(Object, new)) == Object);
}

static void testReturnsItsPrototype() {
	FAssert(FSend(Object, prototype) == NULL);
	FObject *clone = FSend(Object, new);
	FAssert(clone != NULL);
	FAssert(FSend(clone, prototype) == Object);
}

void FRunObjectPrototypeTests() {
	FRunTestSuite("FObjectPrototype", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testImplementsTheIdentityMethod),
		FTestCase(testCanBeCloned),
		FTestCase(testReturnsItsPrototype),
		{0},
	});
}
