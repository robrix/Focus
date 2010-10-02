// FObjectTests.c
// Created by Rob Rix on 2010-10-02
// Copyright 2010 Monochrome Industries

#include "Core/FObject.h"
#include "FTestSuite.h"

static FObject *object = NULL;

static void setUp() {
	object = FObjectCreate(NULL);
}


static void testCreation() {
	FAssert(object != NULL);
}


void FRunObjectTests() {
	FRunTestSuite("FObject", setUp, NULL, (FTestSuiteTestFunction[]){
		testCreation,
		NULL,
	});
}
