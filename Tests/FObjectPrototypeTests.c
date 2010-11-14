// FObjectPrototypeTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/FFunction.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FAssert(FObjectPrototypeGet() != NULL);
}

static void testImplementsTheIdentityMethod() {
	FAssert(FObjectPrototypeGet() == FSend(FObjectPrototypeGet(), self));
}

static void testCanBeCloned() {
	FAssert(FObjectGetPrototype(FSend(FObjectPrototypeGet(), new)) == FObjectPrototypeGet());
}

static void testReturnsItsPrototype() {
	FAssert(FSend(FObjectPrototypeGet(), prototype) == NULL);
	FObject *clone = FSend(FObjectPrototypeGet(), new);
	FAssert(clone != NULL);
	FAssert(FSend(clone, prototype) == FObjectPrototypeGet());
}

void FRunObjectPrototypeTests() {
	FRunTestSuite("FObjectPrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testImplementsTheIdentityMethod),
		FTestCase(testCanBeCloned),
		FTestCase(testReturnsItsPrototype),
		{0},
	});
}
