// FFunctionPrototypeTests.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "Core/Prototypes/FListNodePrototype.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FAssert(FFunctionPrototypeGet() != NULL);
}

static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(FFunctionPrototypeGet()) == FObjectPrototypeGet());
}


static void testCompilesLazily() {
	FObject *receiver = FObjectCreate(NULL);
	FObject *context = FSend(FTestEvaluator, Context);
	FObject *function = FSend(FFunctionPrototypeGet(), newWithContext:arguments:messages:, context, NULL, FListNodeCreateWithObject(FMessageCreateNullaryWithSubstring(NULL, "Object", 6)));
	
	FObjectSetMethod(receiver, FSymbolCreateWithString("test"), function);
	FAssert(FSend(receiver, test) == FSend(context, Object));
}


void FRunFunctionPrototypeTests() {
	FRunTestSuite("FFunctionPrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCreatesASingletonPrototype),
		FTestCase(testInheritsFromObject),
		
		FTestCase(testCompilesLazily),
		
		{0},
	});
}
