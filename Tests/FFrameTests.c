// FFrameTests.c
// Created by Rob Rix on 2011-04-11
// Copyright 2011 Monochrome Industries

#include "Core/FObject.h"
#include "Core/FFrame.h"
#include "Core/FReference.h"
#include "FTestSuite.h"

static struct FFrame *frame = NULL;
static void setUp() {
	frame = FFrameCreate("frame");
}

static void tearDown() {
	FFrameDestroy(frame);
	frame = NULL;
}


static void testCanBeCreated() {
	FAssert(frame != NULL);
}


static void testAddsReferences() {
	struct FObject *oneObject = NULL;
	struct FReference *one = FFrameAppendReference(frame, FReferenceCreate((void **)&oneObject, 0));
	FAssert((one != NULL) && (FReferenceGetAddress(one) == (void **)&oneObject) && (FReferenceGetOffset(one) == 0));
	struct FObject *twoObject = NULL;
	struct FReference *two = FFrameAppendReference(frame, FReferenceCreate((void **)&twoObject, 0));
	FAssert((two != NULL) && (FReferenceGetAddress(two) == (void **)&twoObject) && (FReferenceGetOffset(two) == 0));
}


static void countReferences(struct FFrame *frame, struct FReference *reference, void *context) {
	*(size_t *)context = *(size_t *)context + 1;
}

static void testVisitsReferences() {
	size_t count = 0;
	FFrameVisitReferences(frame, countReferences, &count);
	FAssert(count == 0);
	
	testAddsReferences();
	
	FFrameVisitReferences(frame, countReferences, &count);
	FAssert(count == 2);
}


static void countObjects(struct FFrame *frame, struct FObject *object, void *context) {
	*(size_t *)context = *(size_t *)context + 1;
}

static void testVisitsObjects() {
	size_t count = 0;
	FFrameVisitObjects(frame, countObjects, &count);
	FAssert(count == 0);
}


void FRunFrameTests() {
	FRunTestSuite(&(FTestSuite){"FFrame", setUp, tearDown, (FTestCase[]){
		FTestCase(testCanBeCreated),
		
		FTestCase(testAddsReferences),
		FTestCase(testVisitsReferences),
		
		FTestCase(testVisitsObjects),
		{0},
	}});
}
