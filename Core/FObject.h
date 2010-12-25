// FObject.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT
#define F_OBJECT

#include <stdlib.h>

typedef struct FObject FObject;

typedef FObject *(*FImplementation)(FObject *receiver, FObject *selector, ...);

extern FObject *FSymbolCreateWithString(const char *);
extern FImplementation FFunctionGetImplementation(FObject *function);

#define FSendMessage(receiver, selector, ...) ({ __typeof__(receiver) __receiver = (receiver); __typeof__(selector) __selector = (selector); FFunctionGetImplementation(FObjectGetMethod((FObject *)__receiver, __selector))((FObject *)__receiver, __selector, ## __VA_ARGS__); })
// This macro stringifies the selector. If you want to parameterize the selector, use FSendMessage instead.
#define FSend(receiver, selector, ...) FSendMessage(receiver, FSymbolCreateWithString(#selector))

FObject *FObjectCreate(FObject *prototype);

FObject *FObjectGetPrototype(FObject *self);

FObject *FObjectGetMethod(FObject *self, FObject *selector);
void FObjectSetMethod(FObject *self, FObject *selector, FObject *function);

#endif // F_OBJECT
