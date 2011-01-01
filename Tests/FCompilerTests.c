// FCompilerTests.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "Core/FSymbol.h"
#include "Core/FObject.h"
#include "Core/FCompiler.h"
#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FListNodePrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "FTestSuite.h"

static FObject *compiler = NULL;

static void setUp() {
	compiler = FSend(FSend(FTestEvaluator, Context), Compiler);
}

static void testResolvesReferencesToArguments() {
	FObject *function = FSend(FFunctionPrototypeGet(), newWithContext:arguments:messages:, NULL, FListNodeCreateWithObject(FSymbolCreateWithString("x")), FListNodeCreateWithObject(FMessageCreateNullaryWithSubstring(NULL, "x", 1)));
	
	FImplementation implementation = FCompilerCompileFunction(compiler, function);
	FAssert(implementation != NULL);
	FAssert(implementation(NULL, NULL, NULL) == NULL);
	FAssert(implementation(NULL, NULL, compiler) == compiler);
}

static void testClosesOverItsContext() {
	FObject *context = FSend(FTestEvaluator, Context);
	FObject *function = FSend(FFunctionPrototypeGet(), newWithContext:arguments:messages:, context, NULL, FListNodeCreateWithObject(FMessageCreateNullaryWithSubstring(NULL, "Object", 6)));
	
	FImplementation implementation = FCompilerCompileFunction(compiler, function);
	FAssert(implementation != NULL);
	FAssert(implementation(NULL, NULL) == FSend(FSend(FTestEvaluator, Context), Object));
}

void FRunCompilerTests() {
	FRunTestSuite("FCompiler", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testResolvesReferencesToArguments),
		FTestCase(testClosesOverItsContext),
		
		{0},
	});
}
