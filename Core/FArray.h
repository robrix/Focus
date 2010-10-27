// FArray.h
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#ifndef F_ARRAY
#define F_ARRAY

typedef struct FArray {
	size_t count;
	void **elements;
} FArray;

FArray *FArrayCreate(size_t count);

size_t FArrayGetCount(FArray *self);

void *FArrayGetElementAtIndex(FArray *self, size_t index);

#endif // F_ARRAY
