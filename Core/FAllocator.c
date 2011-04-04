// FAllocator.c
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#include "FPage.h"
#include "FAllocator.h"
#include "FObject+Protected.h"
#include <stdint.h>

// there isn’t a fixed # of generations, rather, generations are allocated as needed when collections are done
// objects larger than the generation size need to be handled separately
typedef struct FAllocator {
	struct FPage *generations;
} FAllocator;


FAllocator *FAllocatorCreate() {
	FAllocator *allocator = calloc(1, sizeof(FAllocator));
	allocator->generations = FPageCreate();
	return allocator;
}

void FAllocatorDestroy(FAllocator *allocator) {
	// destroy each generation in the list
}

struct FPage *FAllocatorGetNursery(FAllocator *allocator) {
	return allocator->generations;
}

FObject *FAllocatorAllocateObject(FAllocator *allocator) {
	FObject *object = FPageAllocate(FAllocatorGetNursery(allocator), sizeof(FObject));
	if(!object) {
		FAllocatorCollect(allocator);
		// perform a collection, promoting live objects to the next generation
		// then allocate again in the nursery
	}
	return object;
}

void *FAllocatorAllocate(FAllocator *allocator, size_t bytes) {
	#pragma message("fixme: handle bytes > F_ALLOCATOR_GENERATION_SIZE specially")
	return calloc(1, bytes);
}

void *FAllocatorResizeAllocation(struct FAllocator *allocator, void *allocation, size_t bytes) {
	return realloc(allocation, bytes);
}


void FAllocatorCollect(FAllocator *allocator) {
	
}
