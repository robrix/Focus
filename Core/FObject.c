// FObject.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FObject.h"
#include "FHashTable.h"
#include "FSymbol.h"

#include "FObject+Protected.h"

FObject *FObjectCreate(FObject *prototype) {
	FObject *object = FAllocatorAllocate(NULL, sizeof(FObject));
	object->prototype = prototype;
	object->variables = FHashTableCreate();
	return object;
}


FObject *FObjectGetPrototype(FObject *self) {
	return self->prototype;
}


FObject *FObjectGetVariable(FObject *self, struct FSymbol *selector) {
	return FHashTableGetValueForKey(self->variables, selector);
}

FObject *FObjectSetVariable(FObject *self, struct FSymbol *selector, FObject *other) {
	FHashTableSetValueForKey(self->variables, selector, other);
	return other;
}


struct FFunction *FObjectGetMethod(FObject *self, struct FSymbol *selector) {
	return (struct FFunction *)(self->methods ? FHashTableGetValueForKey(self->methods, selector) : NULL) ?: (self->prototype ? FObjectGetMethod(self->prototype, selector) : NULL);
}

void FObjectSetMethod(FObject *self, struct FSymbol *selector, struct FFunction *function) {
	if(self->methods == NULL) {
		self->methods = FHashTableCreate();
	}
	FHashTableSetValueForKey(self->methods, selector, function);
}
