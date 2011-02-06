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


FObject *FObjectGetVariable(FObject *self, FSymbol *selector) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	return FHashTableGetValueForKey(&(self->slots), selector);
}

FObject *FObjectSetVariable(FObject *self, FSymbol *selector, FObject *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	
	#pragma message("fixme: implement a write barrier (for updating the remembered set)")
	FHashTableSetValueForKey(&(self->slots), selector, other);
	return other;
}

FObject *FObjectSetVariableAsAccessor(FObject *self, FSymbol *selector, FObject *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(selector != NULL);
	
	const char *string = FSymbolGetString(selector);
	return FObjectSetVariable(self, FSymbolCreateWithSubstring(string, strlen(string) - 1), other);
}


FObject *FObjectGetMethod(FObject *self, FSymbol *selector) {
	return FObjectGetVariable(self, selector);
}

void FObjectSetMethod(FObject *self, FSymbol *selector, FObject *function) {
	FObjectSetVariable(self, selector, function);
}
