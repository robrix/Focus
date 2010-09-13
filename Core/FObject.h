// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>

typedef struct FSymbol * FSymbolRef;

typedef struct FObject {
	struct FObject *prototype;
	struct FHashTable *variables;
	struct FHashTable *methods;
} FObject;

typedef FObject *(*FMethod)(FObject *receiver, struct FSymbol *selector, ...);

FObject *FObjectCreate(FObject *prototype, size_t size);

FMethod FObjectGetMethod(FObject *self, struct FSymbol *selector);
void FObjectSetMethod(FObject *self, struct FSymbol *selector, FMethod method);

FObject *FObjectGetVariable(FObject *self, struct FSymbol *name);
void FObjectSetVariable(FObject *self, struct FSymbol *name, FObject *variable);

#endif // F_OBJECT
