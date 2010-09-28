// FAllocator.h
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#ifndef F_ALLOCATOR
#define F_ALLOCATOR

#include <stdlib.h>

typedef struct FAllocator {
	
} FAllocator;

void *FAllocatorAllocate(FAllocator *allocator, size_t bytes);

#endif // F_ALLOCATOR
