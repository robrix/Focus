// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>

extern struct FSymbol *FSymbolCreateWithString(const char *);

typedef struct FObject FObject;

#define FSend(receiver, selector, ...) FFunctionGetFunctionPointer(FObjectGetMethod((FObject *)receiver, FSymbolCreateWithString(#selector)))((FObject *)receiver, FSymbolCreateWithString(#selector), ## __VA_ARGS__)

FObject *FObjectCreate(FObject *prototype);

FObject *FObjectGetPrototype(FObject *self);

struct FFunction *FObjectGetMethod(FObject *self, struct FSymbol *selector);
void FObjectSetMethod(FObject *self, struct FSymbol *selector, struct FFunction *function);

#endif // F_OBJECT
