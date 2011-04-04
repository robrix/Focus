// FocusTests.c
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#include <stdio.h>
#include <string.h>
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
extern void FRunPageTests();
extern void FRunAllocatorTests();

int main(int argc, const char *argv[]) {
	const char *suite = NULL;
	if((argc == 2) && (strcmp(argv[1], "All") != 0) && (strcmp(argv[1], "") != 0)) {
		suite = argv[1];
		printf("Testing %s\n", suite);
	}
	#define FRunTestsForSuite(suiteName) if((suite == NULL) || (strcmp(suite, #suiteName) == 0)) FRun ## suiteName ## Tests();
	FRunTestsForSuite(Allocator);
	FRunTestsForSuite(Page);
	FRunTestsForSuite(HashTable);
	FRunTestsForSuite(Symbol);
	FRunTestsForSuite(Object);
	FRunTestsForSuite(ObjectPrototype);
	FRunTestsForSuite(ContextPrototype);
	FRunTestsForSuite(FunctionPrototype);
	FRunTestsForSuite(Parser);
	FRunTestsForSuite(Compiler);
	FRunTestsForSuite(ListNodePrototype);
	
	printf("%u suites, %u cases, %u assertions, %u failures\n", FTestSuiteTestSuitesRun, FTestSuiteTestCasesRun, FTestSuiteAssertionsRun, FTestSuiteAssertionsFailed);
	return FTestSuiteAssertionsFailed;
}
