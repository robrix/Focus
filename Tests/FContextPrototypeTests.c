// FContextPrototypeTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "FTestSuite.h"

static FObject *Context = NULL;

static void setUp() {
	FSetUpTestEvaluator();
	Context = FSend(FTestEvaluator, Context);
}

static void testCreatesASingletonPrototype() {
	FAssert(Context != NULL);
}

static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(Context) == FSend(Context, Object));
}

static void testExposesObjectPrototype() {
	FAssert(FSend(Context, Object) == FSend(Context, Object));
}

static void testExposesContextPrototype() {
	FAssert(FSend(Context, Context) == Context);
}

void FRunContextPrototypeTests() {
	FRunTestSuite(&(FTestSuite){"FContextPrototype", setUp, FTearDownTestEvaluator, (FTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testInheritsFromObject),
		FTestCase(testExposesObjectPrototype),
		FTestCase(testExposesContextPrototype),
		{0},
	}});
}
