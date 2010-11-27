// FListNodePrototypeTests.c
// Created by Rob Rix on 2010-11-22
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FListNodePrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/FListNode.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FAssert(FListNodePrototypeGet() != NULL);
}

static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(FListNodePrototypeGet()) == FObjectPrototypeGet());
}

void FRunListNodePrototypeTests() {
	FRunTestSuite("FListNodePrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testInheritsFromObject),
		
		{0},
	});
}
