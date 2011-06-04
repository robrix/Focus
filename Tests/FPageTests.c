// FPageTests.c
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#include "Core/FAllocator.h"
#include "Core/FReference.h"
#include "Core/FPage.h"
#include "FTestSuite.h"

static struct FPage *page = NULL;
static void setUp() {
	page = FPageCreate(NULL);
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


static void testTheMostRecentlyAllocatedObjectCanBeResizedInPlace() {
	struct FObject *object = FPageAllocateObject(page);
	FAssert(FPageCanResizeObjectInPlace(page, object, FObjectGetSizeForSlotCount(1)));
}

static void testObjectsCannotBeResizedPastTheEndOfThePage() {
	struct FObject *object = FPageAllocateObject(page);
	FAssert(!FPageCanResizeObjectInPlace(page, object, F_PAGE_SIZE + 1));
}

static void testLessRecentlyAllocatedObjectsCannotBeResizedInPlace() {
	struct FObject *object = FPageAllocateObject(page);
	FPageAllocateObject(page);
	FAssert(!FPageCanResizeObjectInPlace(page, object, FObjectGetSizeForSlotCount(1)));
}


static void testResizesTheMostRecentlyAllocatedObjectInPlace() {
	struct FObject *object = FPageAllocateObject(page);
	FAssert(FPageGetIndex(page) == FObjectGetSizeForSlotCount(0));
	FAssert(FPageResizeObject(page, object, FObjectGetSizeForSlotCount(1)) == object);
	FAssert(FPageGetIndex(page) == FObjectGetSizeForSlotCount(1));
}


void FRunPageTests() {
	FRunTestSuite(&(FTestSuite){"FPage", setUp, tearDown, (FTestCase[]){
		FTestCase(testAllocatesMemory),
		
		FTestCase(testVisitsAllocatedObjects),
		
		FTestCase(testTheMostRecentlyAllocatedObjectCanBeResizedInPlace),
		FTestCase(testObjectsCannotBeResizedPastTheEndOfThePage),
		FTestCase(testLessRecentlyAllocatedObjectsCannotBeResizedInPlace),
		
		FTestCase(testResizesTheMostRecentlyAllocatedObjectInPlace),
		{0},
	}});
}
