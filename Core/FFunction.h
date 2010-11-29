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

typedef struct FFunction FFunction;

typedef FObject *(*FFunctionPointer)(struct FObject *receiver, struct FSymbol *selector, ...);

FFunction *FFunctionCreateWithFunctionPointer(struct FObject *arguments, FFunctionPointer functionPointer);

FFunctionPointer FFunctionGetFunctionPointer(FFunction *self);
size_t FFunctionGetArity(FFunction *self);

#endif // F_FUNCTION
