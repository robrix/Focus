// FTestSuite.c
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#include "FTestSuite.h"
#include <stdarg.h>

#include "../Core/FEvaluator.h"

unsigned int FTestSuiteAssertionsRun = 0;
unsigned int FTestSuiteAssertionsFailed = 0;
unsigned int FTestSuiteTestCasesRun = 0;
unsigned int FTestSuiteTestSuitesRun = 0;

FObject *FTestEvaluator = NULL;

void FTestSuiteAssertionFailed() {
	FTestSuiteAssertionsFailed++;
}

bool FAssertConditionWithMessage(bool condition, const char *format, ...) {
	FTestSuiteAssertionsRun++;
	if(!condition) {
		va_list arguments;
		va_start(arguments, format);
		vprintf(format, arguments);
		FTestSuiteAssertionFailed();
	}
	fflush(stdout);
	return condition;
}


void FRunTestCase(FTestSuite *suite, FTestCase *testCase) {
	printf("%s\n", testCase->name);
	fflush(stdout);
	
	if(suite->setUp) {
		suite->setUp();
		fflush(stdout);
	}
	
	(testCase->test)();
	fflush(stdout);
	
	if(suite->tearDown) {
		suite->tearDown();
		fflush(stdout);
	}
}

void FRunTestSuite(FTestSuite *suite) {
	printf("Running suite %s\n", suite->name);
	FTestCase *testCase = suite->tests;
	do {
		if(testCase->test) {
			FRunTestCase(suite, testCase);
		}
		testCase++;
		FTestSuiteTestCasesRun++;
	} while(testCase->test);
	printf("\n");
	FTestSuiteTestSuitesRun++;
}

void FSetUpTestEvaluator() {
	FTestEvaluator = FEvaluatorCreate();
}

void FTearDownTestEvaluator() {
	FEvaluatorDestroy(FTestEvaluator);
	FTestEvaluator = NULL;
}
