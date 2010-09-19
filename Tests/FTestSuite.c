// FTestSuite.c
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#include "FTestSuite.h"
#include <stdio.h>

void FRunTestSuite(const char *name, FTestSuiteSetUpFunction setUp, FTestSuiteTearDownFunction tearDown, FTestSuiteTestFunction *tests) {
	printf("Running suite %s\n", name);
	FTestSuiteTestFunction *test = tests;
	do {
		setUp();
		(*test)();
		tearDown();
		test++;
		FTestSuiteTestsRun++;
	} while(*test);
}

int main(int argc, const char *argv[]) {
	printf("%u tests, %u assertions, %u failures\n", FTestSuiteTestsRun, FTestSuiteAssertionsRun, FTestSuiteAssertionsFailed);
	return 0;
}