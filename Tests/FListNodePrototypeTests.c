// FListNodePrototypeTests.c
// Created by Rob Rix on 2010-11-22
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FListNodePrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/FFunction.h"
#include "Core/FListNode.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FObject *prototype = FListNodePrototypeGet();
	FAssert(prototype != NULL);
	FAssert(prototype == FListNodePrototypeGet());
}

static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(FListNodePrototypeGet()) == FObjectPrototypeGet());
}

static void testNewInstancesInheritFromThePrototype() {
	FObject *instance = FSend(FListNodePrototypeGet(), new);
	FAssert(instance != NULL);
	FAssert(FObjectGetPrototype(instance) == FListNodePrototypeGet());
}

static void testInstancesCanBeCreatedWithAnObject() {
	FObject *instance = FSend(FListNodePrototypeGet(), newWith:, FObjectPrototypeGet());
	FAssert(FSend(instance, object) == FObjectPrototypeGet());
	FAssert(FObjectGetPrototype(instance) == FListNodePrototypeGet());
}

void FRunListNodePrototypeTests() {
	FRunTestSuite("FListNodePrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testInheritsFromObject),
		
		FTestCase(testNewInstancesInheritFromThePrototype),
		FTestCase(testInstancesCanBeCreatedWithAnObject),
		{0},
	});
}
