// FAllocator.h
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#ifndef F_ALLOCATOR
#define F_ALLOCATOR

#include <stdlib.h>
#include "FObject.h"

struct FAllocator *FAllocatorCreate();
void FAllocatorDestroy(struct FAllocator *self);

FObject *FAllocatorAllocateObject(struct FAllocator *self);
void *FAllocatorAllocate(struct FAllocator *self, size_t bytes);
void *FAllocatorResizeAllocation(struct FAllocator *self, void *allocation, size_t bytes);

void FAllocatorCollect(struct FAllocator *self);

struct FFrame *FAllocatorGetCurrentFrame(struct FAllocator *self);
struct FFrame *FAllocatorPushFrame(struct FAllocator *self, const char *function);
struct FObject *FAllocatorMakeStrongReferenceToObjectAtAddress(struct FAllocator *self, struct FObject **address);
void FAllocatorPopFrame(struct FAllocator *self);

#endif // F_ALLOCATOR
