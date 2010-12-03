// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>

extern struct FSymbol *FSymbolCreateWithString(const char *);

typedef struct FObject FObject;

// NB: this macro evaluates receiver twice and stringifies selector (meaning you can’t parameterize the selector with this macro)
#define FSend(receiver, selector, ...) FFunctionGetImplementation(FObjectGetMethod((FObject *)receiver, FSymbolCreateWithString(#selector)))((FObject *)receiver, FSymbolCreateWithString(#selector), ## __VA_ARGS__)

FObject *FObjectCreate(FObject *prototype);

FObject *FObjectGetPrototype(FObject *self);

struct FObject *FObjectGetMethod(FObject *self, struct FSymbol *selector);
void FObjectSetMethod(FObject *self, struct FSymbol *selector, struct FObject *function);

#endif // F_OBJECT
