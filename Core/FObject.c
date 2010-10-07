// FObject.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FObject.h"
#include "FHashTable.h"
#include "FSymbol.h"

FObject *FObjectCreate(FObject *prototype) {
	FObject *object = FAllocatorAllocate(NULL, sizeof(FObject));
	object->prototype = prototype;
	return object;
}


FMethod FObjectGetMethod(FObject *self, struct FSymbol *selector) {
	return (FMethod)(self->methods ? FHashTableGetValueForKey(self->methods, selector) : NULL) ?: (self->prototype ? FObjectGetMethod(self->prototype, selector) : NULL);
}

void FObjectSetMethod(FObject *self, struct FSymbol *selector, FMethod method) {
	if(self->methods == NULL) {
		self->methods = FHashTableCreate();
	}
	FHashTableSetValueForKey(self->methods, selector, method);
}
