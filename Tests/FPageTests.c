// FPageTests.c
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#include "Core/FPage.h"
#include "FTestSuite.h"

static struct FPage *page = NULL;
static void setUp() {
	page = FPageCreate();
}

static void tearDown() {
	FPageDestroy(page);
	page = NULL;
}


static void testAllocatesMemory() {
	FAssert(FPageAllocateObject(page) != NULL);
}


static void countObjects(struct FPage *page, FObject *object, void *context) {
	*(uint8_t *)context = *(uint8_t *)context + 1;
}

static void testVisitsAllocatedObjects() {
	uint8_t count = 0;
	FPageVisitObjects(page, countObjects, &count);
	FAssert(count == 0);
	
	FPageAllocateObject(page);
	FPageAllocateObject(page);
	FPageAllocateObject(page);
	
	FPageVisitObjects(page, countObjects, &count);
	FAssert(count == 3);
}


void FRunPageTests() {
	FRunTestSuite(&(FTestSuite){"FPage", setUp, tearDown, (FTestCase[]){
		FTestCase(testAllocatesMemory),
		
		FTestCase(testVisitsAllocatedObjects),
		{0},
	}});
}
