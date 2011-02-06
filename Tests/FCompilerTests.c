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
	FSetUpTestEvaluator();
	compiler = FSend(FSend(FTestEvaluator, Context), Compiler);
}

static void testResolvesReferencesToArguments() {
	FObject *context = FSend(FTestEvaluator, Context);
	FObject *arguments = FSend(FSend(context, ListNode), newWithObject:, FSymbolCreateWithString("x"));
	FObject *message = FSend(FSend(context, Message), newWithReceiver:selector:arguments:, NULL, FSymbolCreateWithString("x"), NULL);
	FObject *messages = FSend(FSend(context, ListNode), newWithObject:, message);
	FObject *function = FSend(FSend(context, Function), newWithContext:arguments:messages:, NULL, arguments, messages);
	
	FImplementation implementation = FCompilerCompileFunction(compiler, function);
	FAssert(implementation != NULL);
	FAssert(implementation(NULL, NULL, NULL) == NULL);
	FAssert(implementation(NULL, NULL, compiler) == compiler);
}

static void testClosesOverItsContext() {
	FObject *context = FSend(FTestEvaluator, Context);
	FObject *message = FSend(FSend(context, Message), newWithReceiver:selector:arguments:, NULL, FSymbolCreateWithString("Object"), NULL);
	FObject *messages = FSend(FSend(context, ListNode), newWithObject:, message);
	FObject *function = FSend(FSend(context, Function), newWithContext:arguments:messages:, context, NULL, messages);
	
	FImplementation implementation = FCompilerCompileFunction(compiler, function);
	FAssert(implementation != NULL);
	FAssert(implementation(NULL, NULL) == FSend(FSend(FTestEvaluator, Context), Object));
}

void FRunCompilerTests() {
	FRunTestSuite(&(FTestSuite){"FCompiler", setUp, FTearDownTestEvaluator, (FTestCase[]){
		FTestCase(testResolvesReferencesToArguments),
		FTestCase(testClosesOverItsContext),
		
		{0},
	}});
}
