// FAllocatorTests.c
// Created by Rob Rix on 2011-01-08
// Copyright 2011 Monochrome Industries

#include "Core/FAllocator.h"
#include "FTestSuite.h"

static struct FAllocator *allocator = NULL;
static void setUp() {
	allocator = FAllocatorCreate();
}

static void tearDown() {
	
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

// zeroing weak refs?

void FRunAllocatorTests() {
	FRunTestSuite(&(FTestSuite){"FAllocator", setUp, tearDown, (FTestCase[]){
		FTestCase(testCanBeCreated),
		
		FTestCase(testAllocatesObjects),
		FTestCase(testCollectsObjects),
		{0},
	}});
}
