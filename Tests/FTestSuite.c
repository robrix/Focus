// FTestSuite.c
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#include "FTestSuite.h"
#include <stdio.h>

unsigned int FTestSuiteAssertionsRun = 0;
unsigned int FTestSuiteAssertionsFailed = 0;
unsigned int FTestSuiteTestCasesRun = 0;
unsigned int FTestSuiteTestSuitesRun = 0;

void FRunTestSuite(const char *name, FTestSuiteSetUpFunction setUp, FTestSuiteTearDownFunction tearDown, FTestSuiteTestCase *tests) {
	printf("Running suite %s\n", name);
	FTestSuiteTestCase *testCase = tests;
	do {
		if(testCase->test) {
			printf("%s\n", testCase->name);
			fflush(stdout);
			
			if(setUp) setUp();
			(testCase->test)();
			if(tearDown) tearDown();
		}
		fflush(stdout);
		testCase++;
		FTestSuiteTestCasesRun++;
	} while(testCase->test);
	printf("\n");
	FTestSuiteTestSuitesRun++;
}
