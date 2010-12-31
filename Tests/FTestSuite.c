// FTestSuite.c
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#include "FTestSuite.h"
#include <stdarg.h>

unsigned int FTestSuiteAssertionsRun = 0;
unsigned int FTestSuiteAssertionsFailed = 0;
unsigned int FTestSuiteTestCasesRun = 0;
unsigned int FTestSuiteTestSuitesRun = 0;


void FAssertionFailed() {
	FTestSuiteAssertionsFailed++;
}

bool FAssertConditionWithMessage(bool condition, const char *format, ...) {
	FTestSuiteAssertionsRun++;
	if(!condition) {
		va_list arguments;
		va_start(arguments, format);
		vprintf(format, arguments);
		FAssertionFailed();
	}
	fflush(stdout);
	return condition;
}


void FRunTestSuite(const char *name, FTestSuiteSetUpFunction setUp, FTestSuiteTearDownFunction tearDown, FTestSuiteTestCase *tests) {
	printf("Running suite %s\n", name);
	FTestSuiteTestCase *testCase = tests;
	do {
		if(testCase->test) {
			printf("%s\n", testCase->name);
			fflush(stdout);
			
			if(setUp) setUp();
			fflush(stdout);
			(testCase->test)();
			fflush(stdout);
			if(tearDown) tearDown();
			fflush(stdout);
		}
		testCase++;
		FTestSuiteTestCasesRun++;
	} while(testCase->test);
	printf("\n");
	FTestSuiteTestSuitesRun++;
}
