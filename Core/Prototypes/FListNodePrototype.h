// FListNodePrototype.h
// Created by Rob Rix on 2010-11-21
// Copyright 2010 Monochrome Industries

#ifndef F_LIST_NODE_PROTOTYPE
#define F_LIST_NODE_PROTOTYPE

#include "../FObject.h"

FObject *FListNodePrototypeGet();

FObject *FListNodeCreateWithObject(FObject *object);
FObject *FListNodeCreateWithObjects(FObject *object, ...);
// FObject *FListNodeCreateWithObjectsAndCount(FObject **objects, size_t count);

size_t FListNodeGetCount(FObject *node);

#endif // F_LIST_NODE_PROTOTYPE
