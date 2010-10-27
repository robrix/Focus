// FArray.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FArray.h"

FArray *FArrayCreate(size_t count) {
	FArray *instance = FAllocatorAllocate(NULL, sizeof(FArray));
	instance->count = count;
	instance->elements = FAllocatorAllocate(NULL, sizeof(void *) * count);
	return instance;
}


size_t FArrayGetCount(FArray *self) {
	return self->count;
}


void *FArrayGetElementAtIndex(FArray *self, size_t index) {
	if(index > self->count) return NULL;
	return self->elements[index];
}
