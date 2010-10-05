// FObjectPrototypeTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FObjectPrototype.h"
#include "FTestSuite.h"

static void testCreatesASingletonPrototype() {
	FAssert(FObjectPrototypeGet() != NULL);
}

static void testRespondsToSelfMessage() {
	FAssert(FObjectPrototypeGet() == FSend(FObjectPrototypeGet(), self));
}

static void testRespondsToNewMessage() {
	FAssert(FSend(FObjectPrototypeGet(), new)->prototype == FObjectPrototypeGet());
}

void FRunObjectPrototypeTests() {
	FRunTestSuite("FObjectPrototype", NULL, NULL, (FTestSuiteTestFunction[]){
		testCreatesASingletonPrototype,
		testRespondsToSelfMessage,
		testRespondsToNewMessage,
		NULL,
	});
}
