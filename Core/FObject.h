// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>

typedef struct FSymbol * FSymbolRef;
extern struct FSymbol *FSymbolCreateWithString(const char *);

typedef struct FObject {
	struct FObject *prototype;
	struct FHashTable *methods;
} FObject;

typedef FObject *(*FMethod)(FObject *receiver, struct FSymbol *selector, ...);

#define FSend(receiver, selector, ...) FObjectGetMethod(receiver, FSymbolCreateWithString(#selector))(receiver, FSymbolCreateWithString(#selector), ## __VA_ARGS__)

FObject *FObjectCreate(FObject *prototype);

FMethod FObjectGetMethod(FObject *self, struct FSymbol *selector);
void FObjectSetMethod(FObject *self, struct FSymbol *selector, FMethod method);

#endif // F_OBJECT
