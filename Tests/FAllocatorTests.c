// FAllocatorTests.c
// Created by Rob Rix on 2011-01-08
// Copyright 2011 Monochrome Industries

#include "Core/FAllocator.h"
#include "Core/FFrame.h"
#include "Core/FObject+Protected.h"
#include "FTestSuite.h"

extern bool FAllocatorObjectIsLive(struct FAllocator *allocator, struct FObject *object);

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
	
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, (void **)&object, 0);
	
	FAllocatorCollect(allocator);
	
	FAssert(address != object);
	
	FAllocatorPopFrame(allocator);
}

static void testUpdatesHeapReferencesOnCollection() {
	FAllocatorPushFrame(allocator, __func__);
	
	struct FObject *object = FAllocatorAllocateObject(allocator);
	struct FObject *container = /*FAllocatorMakeStrongReferenceToObject*/(FAllocatorAllocateObjectWithSlotCount(allocator, 3));
	FObjectSetSlot(container, FSymbolCreateWithString("object"), object);
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, (void **)&container, 0);
	
	struct FObject *address = FObjectGetSlot(container, FSymbolCreateWithString("object"));
	FAssert(address == object);
	
	FAllocatorCollect(allocator);
	
	address = FObjectGetSlot(container, FSymbolCreateWithString("object"));
	FAssert(address != object);
	
	FAllocatorPopFrame(allocator);
}


// static void testObjectsAreConsideredLiveWhenAddedToTheRootSet() {}

static void testObjectsAreConsideredLiveWhenReferencedDirectlyOnTheStack() {
	FAllocatorPushFrame(allocator, __func__);
	
	struct FObject *object = FAllocatorAllocateObject(allocator);
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, (void **)&object, 0);
	
	FAssert(FAllocatorObjectIsLive(allocator, object));
	
	FAllocatorPopFrame(allocator);
}

static void testObjectsAreConsideredLiveWhenReferencedIndirectlyViaTheStack() {
	FAllocatorPushFrame(allocator, __func__);
	
	struct FObject *container = FAllocatorAllocateObjectWithSlotCount(allocator, 3);
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, (void **)&container, 0);
	
	FAssert(FAllocatorObjectIsLive(allocator, container));
	
	struct FObject *object = FAllocatorAllocateObject(allocator);
	
	FAssert(!FAllocatorObjectIsLive(allocator, object));
	
	FObjectSetSlot(container, FSymbolCreateWithString("object"), object);
	FAssert(FAllocatorObjectIsLive(allocator, object));
	
	FAllocatorPopFrame(allocator);
}

static void testObjectsAreConsideredLiveWhenReferencedDirectlyFromAnotherPage() {
	FAllocatorPushFrame(allocator, __func__);
	
	struct FObject *container = FAllocatorAllocateObjectWithSlotCount(allocator, 3);
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, (void **)&container, 0);
	
	FAssert(FAllocatorObjectIsLive(allocator, container));
	
	FAllocatorCollect(allocator);
	
	struct FObject *object = FAllocatorAllocateObject(allocator);
	
	FAssert(!FAllocatorObjectIsLive(allocator, object));
	
	FObjectSetSlot(container, FSymbolCreateWithString("object"), object);
	FAssert(FAllocatorObjectIsLive(allocator, object));
	
	FAllocatorPopFrame(allocator);
}

static void testObjectsAreConsideredLiveWhenReferencedIndirectlyFromAnotherPage() {
	FAllocatorPushFrame(allocator, __func__);
	
	struct FObject *root = FAllocatorAllocateObjectWithSlotCount(allocator, 3);
	FAllocatorMakeStrongReferenceToObjectAtAddress(allocator, (void **)&root, 0);
	
	FAllocatorCollect(allocator);
	
	struct FObject *container = FAllocatorAllocateObjectWithSlotCount(allocator, 3);
	struct FObject *object = FAllocatorAllocateObject(allocator);
	
	FAssert(!FAllocatorObjectIsLive(allocator, container));
	FAssert(!FAllocatorObjectIsLive(allocator, object));
	
	// direct reference from other generation
	FObjectSetSlot(root, FSymbolCreateWithString("container"), container);
	FAssert(FAllocatorObjectIsLive(allocator, container));
	
	// indirect reference from other generation via current generation
	FObjectSetSlot(container, FSymbolCreateWithString("object"), object);
	FAssert(FAllocatorObjectIsLive(allocator, object));
	
	FAllocatorPopFrame(allocator);
}

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


// static void testResizingTheMostRecentAllocationExtendsItInPlace() {
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
		FTestCase(testUpdatesHeapReferencesOnCollection),
		
		// FTestCase(testObjectsAreConsideredLiveWhenAddedToTheRootSet),
		FTestCase(testObjectsAreConsideredLiveWhenReferencedDirectlyOnTheStack),
		FTestCase(testObjectsAreConsideredLiveWhenReferencedIndirectlyViaTheStack),
		FTestCase(testObjectsAreConsideredLiveWhenReferencedDirectlyFromAnotherPage),
		FTestCase(testObjectsAreConsideredLiveWhenReferencedIndirectlyFromAnotherPage),
		
		// FTestCase(testMovingAnAllocationUpdatesHeapReferencesToIt),
		// FTestCase(testMovingAnAllocationUpdatesStackReferencesToIt),
		// FTestCase(testResizingTheMostRecentAllocationExtendsItInPlace),
		// 
		// FTestCase(testVisitsHeapReferencesWithAFunction),
		// FTestCase(testVisitsStackReferencesWithAFunction),
		{0},
	}});
}
