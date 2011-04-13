// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>
#include "FSymbol.h"

typedef struct FObject FObject;

typedef FObject *(*FImplementation)(FObject *receiver, FSymbol *selector, ...);

extern FImplementation FFunctionGetImplementation(FObject *function);

#define FSendMessage(receiver, selector, ...) ({ FObject *__receiver = (receiver); FSymbol *__selector = (selector); FFunctionGetImplementation(FObjectGetMethod(__receiver, __selector))(__receiver, __selector , ## __VA_ARGS__); })
// This macro stringifies the selector. If you want to parameterize the selector, use FSendMessage instead.
#define FSend(receiver, selector, ...) FSendMessage(receiver, FSymbolCreateWithString(#selector) , ## __VA_ARGS__)

FObject *FObjectCreate(FObject *prototype);

FObject *FObjectGetPrototype(FObject *self);

FObject *FObjectGetMethod(FObject *self, FSymbol *selector);
void FObjectSetMethod(FObject *self, FSymbol *selector, FObject *function);

size_t FObjectGetSizeForSlotCount(uint16_t slotCount);
size_t FObjectGetSize(FObject *self);

#endif // F_OBJECT
