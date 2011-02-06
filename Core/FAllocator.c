// FAllocator.c
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FObject+Protected.h"
#include <stdint.h>

// measured in bytes
#define F_ALLOCATOR_GENERATION_SIZE 4096

typedef struct FGeneration {
	uint8_t bytes[F_ALLOCATOR_GENERATION_SIZE];
	size_t index;
	struct FGeneration *next;
} FGeneration;

// there isn’t a fixed # of generations, rather, generations are allocated as needed when collections are done
// the generations should probably be a linked list so that empty ones can be purged more easily (except for the nursery of course)
// objects larger than the generation size need to be handled separately
typedef struct FAllocator {
	FGeneration *generations;
} FAllocator;


FGeneration *FGenerationCreate() {
	FGeneration *generation = calloc(1, sizeof(FGeneration));
	
	return generation;
}

void FGenerationDestroy(FGeneration *generation) {
	
	free(generation);
}

void *FGenerationAllocate(FGeneration *generation, size_t size) {
	void *address = NULL;
	if(generation->index + size <= F_ALLOCATOR_GENERATION_SIZE) {
		address = generation->bytes + generation->index;
		generation->index += size;
	}
	return address;
}


FAllocator *FAllocatorCreate() {
	FAllocator *allocator = calloc(1, sizeof(FAllocator));
	allocator->generations = FGenerationCreate();
	return allocator;
}

void FAllocatorDestroy(FAllocator *allocator) {
	// destroy each generation in the list
}

FGeneration *FAllocatorGetNursery(FAllocator *allocator) {
	return allocator->generations;
}

FObject *FAllocatorAllocateObject(FAllocator *allocator) {
	// allocations > F_ALLOCATOR_GENERATION_SIZE need special handling
	FObject *object = FGenerationAllocate(FAllocatorGetNursery(allocator), sizeof(FObject));
	if(!object) {
		// perform a collection, promoting live objects to the next generation
		// then allocate again in the nursery
	}
	return object;
}

void *FAllocatorAllocate(FAllocator *allocator, size_t bytes) {
	return calloc(1, bytes);
}

void *FAllocatorResizeAllocation(struct FAllocator *allocator, void *allocation, size_t bytes) {
	return realloc(allocation, bytes);
}


void FAllocatorCollect(FAllocator *allocator) {
	
}
