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
	} while(*test);
}
