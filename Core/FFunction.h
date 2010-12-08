// FFunction.h
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#ifndef F_FUNCTION
#define F_FUNCTION

#include "FObject.h"

/*
Interface:
- messages
- arguments
*/

FObject *FFunctionCreateWithImplementation(FObject *arguments, FImplementation functionPointer);

FImplementation FFunctionGetImplementation(FObject *self);
size_t FFunctionGetArity(FObject *self);

#endif // F_FUNCTION
