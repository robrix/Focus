// FCompilerTests.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "Core/FSymbol.h"
#include "Core/FObject.h"
#include "Core/FCompiler.h"
#include "Core/FFunction.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FListNodePrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "FTestSuite.h"

static void testCompilesFunctions() {
	// FObject *function = FSend(FFunctionPrototypeGet(), newWithArguments:messages:, FListNodeCreateWithObject((FObject *)FSymbolCreateWithString("x")), FMessageCreateNullaryWithSubstring(NULL, NULL, "x", 1));
	
	// FCompiler *compiler = FCompilerCreate();
	
	// FCompilerCompileFunction(compiler, function);
	
	// how do we get the fptr? ask the execution engine for it? how does that work? should wrap that in an FCompiler function
	// where does optimization come in? just before getting the fptr?
	
	// call it
	
}

void FRunCompilerTests() {
	FRunTestSuite("FCompiler", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCompilesFunctions),
		
		{0},
	});
}
