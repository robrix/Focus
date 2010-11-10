// FCompilerTests.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "Core/FCompiler.h"
#include "FTestSuite.h"

static void testCompilesMessages() {
	
}

void FRunCompilerTests() {
	FRunTestSuite("FCompiler", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCompilesMessages),
		{0},
	});
}
