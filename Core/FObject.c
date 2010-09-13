// FObject.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FObject.h"
#include "FHashTable.h"
#include "FSymbol.h"

FObject *FObjectCreate(FObject *prototype, size_t size) {
	return NULL;
}


FMethod FObjectGetMethod(FObject *self, struct FSymbol *selector) {
	return FHashTableGetValueForKey(self->methods, selector);
}

void FObjectSetMethod(FObject *self, struct FSymbol *selector, FMethod method) {
	if(self->methods == NULL) {
		self->methods = FHashTableCreate();
	}
	FHashTableSetValueForKey(self->methods, selector, method);
}


FObject *FObjectGetVariable(FObject *self, struct FSymbol *name) {
	return FHashTableGetValueForKey(self->variables, name);
}

void FObjectSetVariable(FObject *self, struct FSymbol *name, FObject *variable) {
	if(self->variables == NULL) {
		self->variables = FHashTableCreate();
	}
	FHashTableSetValueForKey(self->variables, name, variable);
}
