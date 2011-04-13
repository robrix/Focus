// FObject.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FAssertions.h"
#include "FObject+Protected.h"
#include "FSymbol.h"

#include <stdio.h>

FObject *FObjectCreate(FObject *prototype) {
	FObject *object = FAllocatorAllocate(NULL, sizeof(FObject));
	object->prototype = prototype;
	object->slots = FHashTableMake();
	return object;
}


FObject *FObjectGetPrototype(FObject *self) {
	return self->prototype;
}


FObject *FObjectGetSlot(FObject *self, FSymbol *selector) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	return FHashTableGetValueForKey(&(self->slots), selector);
}

FObject *FObjectSetSlot(FObject *self, FSymbol *selector, FObject *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	
	#pragma message("fixme: implement a write barrier (for updating the remembered set)")
	FHashTableSetValueForKey(&(self->slots), selector, other);
	return other;
}


FObject *FObjectGetVariable(FObject *self, FSymbol *selector) {
	return FObjectGetSlot(self, selector);
}

FObject *FObjectSetVariable(FObject *self, FSymbol *selector, FObject *other) {
	return FObjectSetSlot(self, selector, other);
}

FObject *FObjectSetVariableAsAccessor(FObject *self, FSymbol *selector, FObject *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	
	const char *string = FSymbolGetString(selector);
	return FObjectSetVariable(self, FSymbolCreateWithSubstring(string, strlen(string) - 1), other);
}


FObject *FObjectGetMethod(FObject *self, FSymbol *selector) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	return FObjectGetSlot(self, selector) ?: (self->prototype? FObjectGetMethod(self->prototype, selector) : NULL);
}

void FObjectSetMethod(FObject *self, FSymbol *selector, FObject *function) {
	FObjectSetSlot(self, selector, function);
}


size_t FObjectGetSizeForSlotCount(uint16_t slotCount) {
	return
		sizeof(FObject *) // prototype
	+	FHashTableGetSizeForSlotCount(slotCount) // slots
	;
}

size_t FObjectGetSize(FObject *self) {
	return FObjectGetSizeForSlotCount(self? self->slots.slotCount : 0);
}
