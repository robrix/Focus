// FObject+Protected.h
// Created by Rob Rix on 2010-11-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT_PROTECTED
#define F_OBJECT_PROTECTED

#include <stdint.h>
#include <stdlib.h>
#include "FHashTable.h"
#include "FObject.h"

struct FObject {
	struct FObject *prototype;
	FHashTable slots;
};

extern FObject *FObjectGetSlot(FObject *self, FSymbol *selector);
extern FObject *FObjectSetSlot(FObject *self, FSymbol *selector, FObject *variable);

extern FObject *FObjectGetVariable(FObject *self, FSymbol *selector);
extern FObject *FObjectSetVariable(FObject *self, FSymbol *selector, FObject *variable);

extern FObject *FObjectSetVariableAsAccessor(FObject *self, FSymbol *selector, FObject *other);

#endif // F_OBJECT_PROTECTED
