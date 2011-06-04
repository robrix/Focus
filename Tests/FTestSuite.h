// FTestSuite.h
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#ifndef F_TEST_SUITE
#define F_TEST_SUITE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../Core/FObject.h"

extern FObject *FTestEvaluator;

typedef void (*FTestSuiteSetUpFunction)();
typedef void (*FTestSuiteTearDownFunction)();
typedef void (*FTestCaseFunction)();

typedef struct FTestCase {
	FTestCaseFunction test;
	const char *name;
} FTestCase;

typedef struct FTestSuite {
	const char *name;
	FTestSuiteSetUpFunction setUp;
	FTestSuiteTearDownFunction tearDown;
	FTestCase *tests;
} FTestSuite;

extern unsigned int FTestSuiteAssertionsRun;
extern unsigned int FTestSuiteAssertionsFailed;
extern unsigned int FTestSuiteTestCasesRun;
extern unsigned int FTestSuiteTestSuitesRun;

void FRunTestCase(FTestSuite *suite, FTestCase *testCase);
void FRunTestSuite(FTestSuite *suite);

void FSetUpTestEvaluator();
void FTearDownTestEvaluator();

#define FFailWithOptionalMessageString(ignored, format, ...) { printf((format), ## __VA_ARGS__); FTestSuiteAssertionsFailed++; }

#define FAssert(_expression, ...) FAssertConditionWithMessage((bool)(_expression), ## __VA_ARGS__, "%s:%u: error: %s was unexpectedly false.\n", __FILE__, __LINE__, #_expression)

#define FFail(format, ...) { FTestSuiteAssertionsRun++; FFailWithOptionalMessageString(, format, ## __VA_ARGS__); }


#define FTestCase(testCase) (FTestCase){testCase, #testCase}


bool FAssertConditionWithMessage(bool condition, const char *format, ...);


#ifdef __clang__
	// this is bad, as strict aliasing will break it, but clang doesn’t handle union casts correctly
	#define FCast(x, toType) *(toType *)&(x)
#else
	#define FCast(x, toType) (((union{__typeof__(x) a; toType b;})x).b)
#endif
#define FRound(value, place) (round((value) / (place)) * (place))

#endif // F_TEST_SUITE
