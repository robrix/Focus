// FFunctionPrototypeTests.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FAssert(FFunctionPrototypeGet() != NULL);
}

static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(FFunctionPrototypeGet()) == FObjectPrototypeGet());
}

void FRunFunctionPrototypeTests() {
	FRunTestSuite("FFunctionPrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testInheritsFromObject),
		{0},
	});
}
