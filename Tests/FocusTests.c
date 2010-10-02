// FocusTests.c
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#include <stdio.h>
#include "FTestSuite.h"

extern void FRunHashTableTests();
extern void FRunSymbolTests();

int main(int argc, const char *argv[]) {
	FRunHashTableTests();
	FRunSymbolTests();
	printf("%u suites, %u cases, %u assertions, %u failures\n", FTestSuiteTestSuitesRun, FTestSuiteTestCasesRun, FTestSuiteAssertionsRun, FTestSuiteAssertionsFailed);
	return FTestSuiteAssertionsFailed;
}
