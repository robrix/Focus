// FTestSuite.h
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#ifndef F_TEST_SUITE
#define F_TEST_SUITE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void (*FTestSuiteSetUpFunction)();
typedef void (*FTestSuiteTearDownFunction)();
typedef void (*FTestSuiteTestFunction)();

typedef struct FTestSuiteTestCase {
	FTestSuiteTestFunction test;
	const char *name;
} FTestSuiteTestCase;

extern unsigned int FTestSuiteAssertionsRun;
extern unsigned int FTestSuiteAssertionsFailed;
extern unsigned int FTestSuiteTestCasesRun;
extern unsigned int FTestSuiteTestSuitesRun;

void FRunTestSuite(const char *name, FTestSuiteSetUpFunction setUp, FTestSuiteTearDownFunction tearDown, FTestSuiteTestCase *tests);

#define FFailWithOptionalMessageString(ignored, format, ...) { printf((format), ## __VA_ARGS__); FTestSuiteAssertionsFailed++; }

#define FAssert(_expression, ...) {\
	__typeof__(_expression) __condition = (_expression);\
	FTestSuiteAssertionsRun++;\
	if(!__condition)\
		FFailWithOptionalMessageString(, ## __VA_ARGS__, "%s:%u: error: %s was unexpectedly false.\n", __FILE__, __LINE__, #_expression);\
}
#undef FAssert
#define FAssert(_expression, ...) FAssertConditionWithMessage((bool)(_expression), ## __VA_ARGS__, "%s:%u: error: %s was unexpectedly false.\n", __FILE__, __LINE__, #_expression)

#define FFail(format, ...) { FTestSuiteAssertionsRun++; FFailWithOptionalMessageString(, format, ## __VA_ARGS__); }


#define FTestCase(testCase) (FTestSuiteTestCase){testCase, #testCase}


bool FAssertConditionWithMessage(bool condition, const char *format, ...);


#ifdef __clang__
	// this is bad, as strict aliasing will break it, but clang doesn’t handle union casts correctly
	#define FCast(x, toType) *(toType *)&(x)
#else
	#define FCast(x, toType) (((union{__typeof__(x) a; toType b;})x).b)
#endif
#define FRound(value, place) (round((value) / (place)) * (place))

#endif // F_TEST_SUITE
