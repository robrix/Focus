// FObject.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FObject.h"
#include "FHashTable.h"
#include "FSymbol.h"

struct FObject {
	struct FObject *prototype;
	struct FHashTable *slots;
};


FObject *FObjectCreate(FObject *prototype) {
	FObject *object = FAllocatorAllocate(NULL, sizeof(FObject));
	object->prototype = prototype;
	return object;
}


FObject *FObjectGetPrototype(FObject *self) {
	return self->prototype;
}


FMethod FObjectGetSlot(FObject *self, struct FSymbol *selector) {
	return (FMethod)(self->slots ? FHashTableGetValueForKey(self->slots, selector) : NULL) ?: (self->prototype ? FObjectGetSlot(self->prototype, selector) : NULL);
}

void FObjectSetSlot(FObject *self, struct FSymbol *selector, FMethod method) {
	if(self->slots == NULL) {
		self->slots = FHashTableCreate();
	}
	FHashTableSetValueForKey(self->slots, selector, method);
}
