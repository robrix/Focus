// FAllocatorTests.c
// Created by Rob Rix on 2011-01-08
// Copyright 2011 Monochrome Industries

#include "Core/FAllocator.h"
#include "Core/FFrame.h"
#include "FTestSuite.h"

static struct FAllocator *allocator = NULL;
static void setUp() {
	allocator = FAllocatorCreate();
}

static void tearDown() {
	FAllocatorDestroy(allocator);
	allocator = NULL;
}


static void testCanBeCreated() {
	FAssert(allocator != NULL);
}


static void testAllocatesObjects() {
	FObject *object = FAllocatorAllocateObject(allocator);
	FAssert(object != NULL);
}

static void testCollectsObjects() {
	FAllocatorCollect(allocator);
}


static void testPushesStackFrames() {
	struct FFrame *one = FAllocatorPushFrame(allocator, "one");
	FAssert((one != NULL) && (strcmp(FFrameGetName(one), "one") == 0));
	FAssert(FAllocatorGetCurrentFrame(allocator) == one);
	struct FFrame *two = FAllocatorPushFrame(allocator, "two");
	FAssert(FAllocatorGetCurrentFrame(allocator) == two);
	FAssert((two != NULL) && (FFrameGetPreviousFrame(two) == one));
}

static void testPopsStackFrames() {
	struct FFrame *one = FAllocatorPushFrame(allocator, "one");
	struct FFrame *two = FAllocatorPushFrame(allocator, "two");
	FAssert(FAllocatorGetCurrentFrame(allocator) == two);
	FAllocatorPopFrame(allocator);
	FAssert(FAllocatorGetCurrentFrame(allocator) == one);
	FAllocatorPopFrame(allocator);
	FAssert(FAllocatorGetCurrentFrame(allocator) == NULL);
}


static void testUpdatesStackReferencesOnCollection() {
	FAllocatorPushFrame(allocator, __func__);
	
	struct FObject *object = FAllocatorAllocateObject(allocator);
	struct FObject *address = object;
	
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, &object);
	
	FAllocatorCollect(allocator);
	
	FAssert(address != object);
	
	FAllocatorPopFrame(allocator);
}



/*
	moving an allocation
*/
// static void testMovingAnAllocationUpdatesHeapReferencesToIt() {
// 	// FObject
// 	// 	*referenceObject = FAllocatorAllocateObject(allocator),
// 	// 	*referencingObject = FAllocatorAllocateObject(allocator);
// 	// referencingObject
// 	// make a new reference object
// 	// make a new object referencing the original object
// 	// move the original object
// 	// verify that it’s been moved
// 	// verify that the new object has been updated to point at its new address
// }
// 
// static void testMovingAnAllocationUpdatesStackReferencesToIt() {
// 	
// }
// 
// static void testResizingTheMostRecentAllocationExtendsItInPlace() {
// 	
// }
// 
// 
// static void testVisitsHeapReferencesWithAFunction() {
// 	
// }
// 
// static void testVisitsStackReferencesWithAFunction() {
// 	
// }

// zeroing weak refs?

void FRunAllocatorTests() {
	FRunTestSuite(&(FTestSuite){"FAllocator", setUp, tearDown, (FTestCase[]){
		FTestCase(testCanBeCreated),
		
		FTestCase(testAllocatesObjects),
		FTestCase(testCollectsObjects),
		
		FTestCase(testPushesStackFrames),
		FTestCase(testPopsStackFrames),
		
		FTestCase(testUpdatesStackReferencesOnCollection),
		
		// FTestCase(testMovingAnAllocationUpdatesHeapReferencesToIt),
		// FTestCase(testMovingAnAllocationUpdatesStackReferencesToIt),
		// FTestCase(testResizingTheMostRecentAllocationExtendsItInPlace),
		// 
		// FTestCase(testVisitsHeapReferencesWithAFunction),
		// FTestCase(testVisitsStackReferencesWithAFunction),
		{0},
	}});
}
