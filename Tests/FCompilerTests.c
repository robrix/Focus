// FCompilerTests.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "Core/FSymbol.h"
#include "Core/FObject.h"
#include "Core/FCompiler.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FListNodePrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "FTestSuite.h"

static void testCompilesFunctions() {
	// fixme: test it with a given context
	FObject *function = FSend(FFunctionPrototypeGet(), newWithContext:arguments:messages:, NULL, FListNodeCreateWithObject(FSymbolCreateWithString("x")), FListNodeCreateWithObject(FMessageCreateNullaryWithSubstring(NULL, "x", 1)));
	
	FObject *compiler = FCompilerCreate();
	
	FImplementation implementation = FCompilerCompileFunction(compiler, function);
	FAssert(implementation != NULL);
	
	// where does optimization come in? just before getting the fptr?
	
	// call it
	/*
	calling conventions:
	- implementations take their receiver as the first argument, and the selector they are being called with as the second. These may be null.
	- functions are created with a reference to a context (scope).
	- when a function is called, it sets up a new, temporary context inheriting from its original context.
	- nullary messages within the function’s body are compared against its arguments.
	*/
	FAssert(implementation(NULL, NULL, NULL) == NULL);
	FAssert(implementation(NULL, NULL, FObjectPrototypeGet()) == FObjectPrototypeGet());
}

void FRunCompilerTests() {
	FRunTestSuite("FCompiler", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCompilesFunctions),
		
		{0},
	});
}
