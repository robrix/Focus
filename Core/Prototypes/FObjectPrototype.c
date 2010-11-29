// FObjectPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FObjectPrototype.h"

#include "../FSymbol.h"
#include "../FFunction.h"
#include <stdlib.h>

static FObject *FObjectPrototype = NULL;

FObject *FObjectPrototypeGetSelf(FObject *self, FSymbol *selector) {
	return self;
}

FObject *FObjectPrototypeGetPrototype(FObject *self, FSymbol *selector) {
	return FObjectGetPrototype(self);
}

FObject *FObjectPrototypeClone(FObject *self, FSymbol *selector) {
	return FObjectCreate(self);
}


FObject *FObjectPrototypeGet() {
	if(!FObjectPrototype) {
		FObjectPrototype = FObjectCreate(NULL);
		FObjectSetMethod(FObjectPrototype, FSymbolCreateWithString("self"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectPrototypeGetSelf));
		FObjectSetMethod(FObjectPrototype, FSymbolCreateWithString("prototype"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectPrototypeGetPrototype));
		FObjectSetMethod(FObjectPrototype, FSymbolCreateWithString("new"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectPrototypeClone));
	}
	return FObjectPrototype;
}
