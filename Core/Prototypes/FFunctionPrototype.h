// FFunctionPrototype.h
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#ifndef F_FUNCTION_PROTOTYPE
#define F_FUNCTION_PROTOTYPE

#include <stddef.h>
#include "../FObject.h"

FObject *FFunctionPrototypeGet();

extern const size_t FFunctionArgumentNotFound; // size_t is unsigned, so -1 is all 1s.
size_t FFunctionGetIndexOfArgument(FObject *function, FObject *symbol);

#endif
