// FPage.c
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#include "FPage.h"

#include <stdint.h>

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

void FPageDestroy(FPage *page) {
	free(page);
}


void *FPageAllocate(FPage *page, size_t size) {
	void *address = NULL;
	if(page->index + size <= F_PAGE_SIZE) {
		address = page->bytes + page->index;
		page->index += size;
	}
	return address;
}

FObject *FPageAllocateObject(FPage *page) {
	return FPageAllocate(page, FObjectGetSize(NULL));
}


void FPageVisitObjects(FPage *page, FPageObjectVisitor visitor, void *context) {
	size_t index = 0;
	while(index < page->index) {
		FObject *object = ((void *)(page->bytes)) + index;
		visitor(page, object, context);
		index += FObjectGetSize(object);
	}
}
