// FPage.h
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#ifndef F_PAGE
#define F_PAGE

#include "FObject.h"

// measured in bytes
#define F_PAGE_SIZE 4096

// struct FPage *FPageForAddress(void *address);

struct FPage *FPageCreate(struct FAllocator *allocator);
void FPageDestroy(struct FPage *self);

size_t FPageGetIndex(struct FPage *self);

struct FPage *FPageGetNextPage(struct FPage *self);
struct FAllocator *FPageGetAllocator(struct FPage *self);

struct FObject *FPageAllocateObjectWithSlotCount(struct FPage *self, uint16_t slotCount);
struct FObject *FPageAllocateObject(struct FPage *self);
struct FObject *FPageCopyObject(struct FPage *self, struct FObject *original);

bool FPageContainsAddress(struct FPage *self, void *address);

// the most recently allocated object can be resized in place
// todo: objects with sufficient free space following them, e.g. nulled-out allocations, can be resized in place (if we can correctly mark the remaining space as being blank)
bool FPageCanResizeObjectInPlace(struct FPage *self, struct FObject *object, size_t newSize);
// resizes the object, in place if possible, and returns its new address. does not change the contents of the object at all, so FObjectGetSize() will not report the new size!
struct FObject *FPageResizeObject(struct FPage *self, struct FObject *object, size_t newSize);

void FPageDrain(struct FPage *self);

typedef void (*FPageObjectVisitor)(struct FPage *self, struct FObject *object, void *context);
void FPageVisitObjects(struct FPage *self, FPageObjectVisitor visitor, void *context);

typedef void (*FPageReferenceVisitor)(struct FPage *self, struct FObject *object, struct FReference *reference, void *context);
void FPageVisitReferences(struct FPage *self, FPageReferenceVisitor visitor, void *context);

struct FPage *FPageListGetLastPage(struct FPage *self);
struct FPage *FPageListAppendPage(struct FPage *self, struct FPage *other);

typedef void (*FPageListPageVisitor)(struct FPage *page, void *context);
void FPageListVisitPages(struct FPage *self, FPageListPageVisitor visitor, void *context);

#endif // F_PAGE
