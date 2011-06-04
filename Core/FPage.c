// FPage.c
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"
#include "FAllocator.h"
#include "FObject+Protected.h"
#include "FReference.h"
#include "FPage.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct FPage {
	uint8_t bytes[F_PAGE_SIZE];
	size_t index;
	struct FAllocator *allocator;
	struct FPage *next;
} FPage;


struct FPage *FPageCreate(struct FAllocator *allocator) {
	FPage *page = calloc(1, sizeof(FPage));
	page->allocator = allocator;
	return page;
}

void FPageDestroy(struct FPage *self) {
	free(self);
}


size_t FPageGetIndex(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return self->index;
}


struct FPage *FPageGetNextPage(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return self->next;
}

struct FAllocator *FPageGetAllocator(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return self->allocator;
}


void *FPageAllocate(struct FPage *self, size_t size) {
	FAssertPrecondition(self != NULL);
	void *address = NULL;
	if(self->index + size <= F_PAGE_SIZE) {
		address = self->bytes + self->index;
		self->index += size;
	}
	return address;
}

struct FObject *FPageAllocateObjectWithSlotCount(struct FPage *self, uint16_t slotCount) {
	FAssertPrecondition(self != NULL);
	struct FObject *object = FPageAllocate(self, FObjectGetSizeForSlotCount(slotCount));
	object->slots.slotCount = slotCount;
	return object;
}

struct FObject *FPageAllocateObject(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return FPageAllocateObjectWithSlotCount(self, 0);
}

struct FObject *FPageCopyObject(struct FPage *self, struct FObject *original) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(original != NULL);
	size_t size = FObjectGetSize(original);
	return memcpy(FPageAllocate(self, size), original, size);
}


bool FPageContainsAddress(struct FPage *self, void *address) {
	FAssertPrecondition(self != NULL);
	return
		((void *)self <= address)
	&&	(address < (void *)self + F_PAGE_SIZE);
}


size_t FPageIndexOfObject(struct FPage *self, struct FObject *object) {
	return (size_t)object - (size_t)self;
}

bool FPageCanResizeObjectInPlace(struct FPage *self, struct FObject *object, size_t newSize) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(object != NULL);
	size_t objectIndex = FPageIndexOfObject(self, object);
	return
		((objectIndex + FObjectGetSize(object)) == self->index)
	&&	((objectIndex + newSize) < F_PAGE_SIZE);
}

struct FObject *FPageResizeObject(struct FPage *self, struct FObject *object, size_t newSize) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(object != NULL);
	if(FPageCanResizeObjectInPlace(self, object, newSize)) {
		self->index = FPageIndexOfObject(self, object) + newSize;
	} else {
		
	}
	return object;
}


void FPageDrain(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	self->index = 0;
}


void FPageVisitObjects(struct FPage *self, FPageObjectVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(visitor != NULL);
	size_t index = 0;
	while(index < self->index) {
		FObject *object = ((void *)(self->bytes)) + index;
		visitor(self, object, context);
		index += FObjectGetSize(object);
	}
}


struct FPageReferenceVisitorState {
	FPageReferenceVisitor visitor;
	void *context;
	struct FPage *page;
	struct FObject *object;
};

void FPageVisitReferenceInSlot(FHashTable *table, FSlot *slot, void *context) {
	struct FPageReferenceVisitorState *state = context;
	if(slot->value != NULL) {
		struct FReference *reference = FReferenceCreate(&(slot->value), 0);
		state->visitor(state->page, state->object, reference, state->context);
		FReferenceDestroy(reference);
	}
}

void FPageVisitReferencesInObject(struct FPage *self, FObject *object, void *context) {
	struct FPageReferenceVisitorState *state = context;
	state->object = object;
	if(object->prototype != NULL) {
		struct FReference *reference = FReferenceCreate((void **)&(object->prototype), 0);
		state->visitor(state->page, object, reference, state->context);
		FReferenceDestroy(reference);
	}
	FHashTableVisitSlots(&object->slots, FPageVisitReferenceInSlot, context);
}

void FPageVisitReferences(struct FPage *self, FPageReferenceVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(visitor != NULL);
	struct FPageReferenceVisitorState state = {
		.visitor = visitor,
		.context = context,
		.page = self
	};
	FPageVisitObjects(self, FPageVisitReferencesInObject, &state);
}


struct FPage *FPageListGetLastPage(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return FPageGetNextPage(self)?
		FPageListGetLastPage(FPageGetNextPage(self))
	:	self;
}

struct FPage *FPageListAppendPage(struct FPage *self, struct FPage *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(other != NULL);
	FAssertPrecondition(self != other);
	
	return FPageListGetLastPage(self)->next = other;
}


void FPageListVisitPages(struct FPage *self, FPageListPageVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(visitor != NULL);
	while(self != NULL) {
		visitor(self, context);
		self = FPageGetNextPage(self);
	}	
}
