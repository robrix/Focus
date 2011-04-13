// FAllocator.h
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#ifndef F_ALLOCATOR
#define F_ALLOCATOR

#include <stdlib.h>
#include "FObject.h"

struct FAllocator *FAllocatorCreate();
void FAllocatorDestroy(struct FAllocator *self);

struct FObject *FAllocatorAllocateObjectWithSlotCount(struct FAllocator *self, uint16_t slotCount);
struct FObject *FAllocatorAllocateObject(struct FAllocator *self);
void *FAllocatorAllocate(struct FAllocator *self, size_t bytes);
void *FAllocatorResizeAllocation(struct FAllocator *self, void *allocation, size_t bytes);

void FAllocatorCollect(struct FAllocator *self);

struct FFrame *FAllocatorGetCurrentFrame(struct FAllocator *self);
struct FFrame *FAllocatorPushFrame(struct FAllocator *self, const char *function);
struct FObject *FAllocatorMakeStrongReferenceToObjectAtAddress(struct FAllocator *self, void **address, size_t offset);
void FAllocatorPopFrame(struct FAllocator *self);

#endif // F_ALLOCATOR
