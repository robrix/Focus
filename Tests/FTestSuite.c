// FTestSuite.c
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#include "FTestSuite.h"
#include <stdio.h>

unsigned int FTestSuiteAssertionsRun = 0;
unsigned int FTestSuiteAssertionsFailed = 0;
unsigned int FTestSuiteTestCasesRun = 0;
unsigned int FTestSuiteTestSuitesRun = 0;

void FRunTestSuite(const char *name, FTestSuiteSetUpFunction setUp, FTestSuiteTearDownFunction tearDown, FTestSuiteTestFunction *tests) {
	printf("Running suite %s\n", name);
	FTestSuiteTestFunction *test = tests;
	do {
		if(setUp) setUp();
		if(*test) (*test)();
		if(tearDown) tearDown();
		fflush(stdout);
		test++;
		FTestSuiteTestCasesRun++;
	} while(*test);
	FTestSuiteTestSuitesRun++;
}
