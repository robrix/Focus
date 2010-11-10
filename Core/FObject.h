// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>

extern struct FSymbol *FSymbolCreateWithString(const char *);

typedef struct FObject FObject;

typedef FObject *(*FMethod)(FObject *receiver, struct FSymbol *selector, ...);

#define FSend(receiver, selector, ...) FObjectGetSlot(receiver, FSymbolCreateWithString(#selector))(receiver, FSymbolCreateWithString(#selector), ## __VA_ARGS__)

FObject *FObjectCreate(FObject *prototype);

FObject *FObjectGetPrototype(FObject *self);

FMethod FObjectGetSlot(FObject *self, struct FSymbol *selector);
void FObjectSetSlot(FObject *self, struct FSymbol *selector, FMethod method);

#endif // F_OBJECT
