// FTestSuite.h
// Created by Rob Rix on 2010-09-15
// Copyright 2010 Monochrome Industries

#ifndef F_TEST_SUITE
#define F_TEST_SUITE

typedef void (*FTestSuiteSetUpFunction)();
typedef void (*FTestSuiteTearDownFunction)();
typedef void (*FTestSuiteTestFunction)();

unsigned int FTestSuiteAssertionsRun = 0;
unsigned int FTestSuiteAssertionsFailed = 0;
unsigned int FTestSuiteTestsRun = 0;

void FRunTestSuite(const char *name, FTestSuiteSetUpFunction setUp, FTestSuiteTearDownFunction tearDown, FTestSuiteTestFunction *tests);

#define FFailWithOptionalMessageString(ignored, format, ...) { printf((format), ## __VA_ARGS__); FTestSuiteAssertionsFailed++; }

#define FAssert(_expression, ...) {\
	__typeof__(_expression) __condition = (_expression);\
	FTestSuiteAssertionsRun++;\
	if(!__condition)\
		FFailWithOptionalMessageString(, ## __VA_ARGS__, "error: %s was unexpectedly false.", #_expression);\
}

#define FFail(format, ...) { FTestSuiteAssertionsRun++; FFailWithOptionalMessageString(, format, ## __VA_ARGS__); }


#ifdef __clang__
	// this is bad, as strict aliasing will break it, but clang doesn’t handle union casts correctly
	#define FCast(x, toType) *(toType *)&(x)
#else
	#define FCast(x, toType) (((union{__typeof__(x) a; toType b;})x).b)
#endif
#define FRound(value, place) (round((value) / (place)) * (place))

#endif // F_TEST_SUITE
