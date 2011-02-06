// FAllocator.h
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#ifndef F_ALLOCATOR
#define F_ALLOCATOR

#include <stdlib.h>
#include "FObject.h"

struct FAllocator *FAllocatorCreate();
void FAllocatorDestroy(struct FAllocator *allocator);

FObject *FAllocatorAllocateObject(struct FAllocator *allocator);
void *FAllocatorAllocate(struct FAllocator *allocator, size_t bytes);
void *FAllocatorResizeAllocation(struct FAllocator *allocator, void *allocation, size_t bytes);

void FAllocatorCollect(struct FAllocator *allocator);

#endif // F_ALLOCATOR
