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


FObject *FObjectGetVariable(FObject *self, struct FObject *selector) {
	return FHashTableGetValueForKey(self->variables, selector);
}

FObject *FObjectGetVariableWithHash(struct FObject *self, struct FObject *selector, size_t hash) {
	return FHashTableGetValueForKeyWithHash(self->variables, selector, hash);
}

FObject *FObjectSetVariable(FObject *self, struct FObject *selector, FObject *other) {
	FHashTableSetValueForKey(self->variables, selector, other);
	return other;
}

FObject *FObjectSetVariableWithHash(struct FObject *self, struct FObject *selector, size_t hash, struct FObject *variable) {
	FHashTableSetValueForKeyWithHash(self->variables, selector, hash, variable);
	return variable;
}

FObject *FObjectSetVariableAsAccessor(FObject *self, struct FObject *selector, FObject *other) {
	const char *string = FSymbolGetString(selector);
	FHashTableSetValueForKey(self->variables, FSymbolCreateWithSubstring(string, strlen(string) - 1), other);
	return other;
}


struct FObject *FObjectGetMethod(FObject *self, struct FObject *selector) {
	return (struct FObject *)(self->methods ? FHashTableGetValueForKey(self->methods, selector) : NULL) ?: (self->prototype ? FObjectGetMethod(self->prototype, selector) : NULL);
}

void FObjectSetMethod(FObject *self, struct FObject *selector, struct FObject *function) {
	if(self->methods == NULL) {
		self->methods = FHashTableCreate();
	}
	FHashTableSetValueForKey(self->methods, selector, function);
}
