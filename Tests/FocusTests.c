// FocusTests.c
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#include <stdio.h>
#include "FTestSuite.h"

extern void FRunHashTableTests();
extern void FRunSymbolTests();
extern void FRunObjectTests();
extern void FRunObjectPrototypeTests();
extern void FRunContextPrototypeTests();
extern void FRunFunctionPrototypeTests();
extern void FRunParserTests();
extern void FRunCompilerTests();
extern void FRunListNodePrototypeTests();

int main(int argc, const char *argv[]) {
	FRunHashTableTests();
	FRunSymbolTests();
	FRunObjectTests();
	FRunObjectPrototypeTests();
	FRunContextPrototypeTests();
	FRunFunctionPrototypeTests();
	FRunParserTests();
	FRunCompilerTests();
	FRunListNodePrototypeTests();
	
	printf("%u suites, %u cases, %u assertions, %u failures\n", FTestSuiteTestSuitesRun, FTestSuiteTestCasesRun, FTestSuiteAssertionsRun, FTestSuiteAssertionsFailed);
	return FTestSuiteAssertionsFailed;
}
