// FFunction.h
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#ifndef F_FUNCTION
#define F_FUNCTION

/*
Interface:
- messages
- arguments
*/

typedef FObject *(*FImplementation)(struct FObject *receiver, struct FSymbol *selector, ...);

FObject *FFunctionCreateWithImplementation(struct FObject *arguments, FImplementation functionPointer);

FImplementation FFunctionGetImplementation(FObject *self);
size_t FFunctionGetArity(FObject *self);

#endif // F_FUNCTION
