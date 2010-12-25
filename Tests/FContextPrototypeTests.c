// FContextPrototypeTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FAssert(FContextPrototypeGet() != NULL);
}

static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(FContextPrototypeGet()) == FObjectPrototypeGet());
}

static void testExposesObjectPrototype() {
	FAssert(FSend(FContextPrototypeGet(), Object) == FObjectPrototypeGet());
}

static void testExposesContextPrototype() {
	FAssert(FSend(FContextPrototypeGet(), Context) == FContextPrototypeGet());
}

void FRunContextPrototypeTests() {
	FRunTestSuite("FContextPrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testInheritsFromObject),
		FTestCase(testExposesObjectPrototype),
		FTestCase(testExposesContextPrototype),
		{0},
	});
}
