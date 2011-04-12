// FPage.c
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"
#include "FObject.h"
#include "FPage.h"

#include <stdint.h>
#include <stdlib.h>

// measured in bytes
#define F_PAGE_SIZE 4096

typedef struct FPage {
	uint8_t bytes[F_PAGE_SIZE];
	size_t index;
	struct FPage *next;
} FPage;


struct FPage *FPageCreate() {
	FPage *page = calloc(1, sizeof(FPage));
	return page;
}

void FPageDestroy(struct FPage *self) {
	free(self);
}


struct FPage *FPageGetNextPage(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return self->next;
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

struct FObject *FPageAllocateObject(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	return FPageAllocate(self, FObjectGetSize(NULL));
}

struct FObject *FPageCopyObject(struct FPage *self, struct FObject *original) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(original != NULL);
	size_t size = FObjectGetSize(original);
	return memcpy(FPageAllocate(self, size), original, size);
}


void FPageDrain(struct FPage *self) {
	FAssertPrecondition(self != NULL);
	self->index = 0;
}


void FPageVisitObjects(struct FPage *self, FPageObjectVisitor visitor, void *context) {
	size_t index = 0;
	while(index < self->index) {
		FObject *object = ((void *)(self->bytes)) + index;
		visitor(self, object, context);
		index += FObjectGetSize(object);
	}
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
