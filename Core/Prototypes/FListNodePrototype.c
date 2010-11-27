// FListNodePrototype.c
// Created by Rob Rix on 2010-11-21
// Copyright 2010 Monochrome Industries

#include "FListNodePrototype.h"

#include "../FSymbol.h"
#include "../FFunction.h"
#include "../FObject+Protected.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

static FObject *FListNodePrototype = NULL;

FObject *FListNodePrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	FObjectSetMethod(prototype, FSymbolCreateWithString("nextNode"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("object"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectGetVariable));
	return prototype;
}

FObject *FListNodePrototypeGet() {
	if(!FListNodePrototype) {
		FListNodePrototype = FListNodePrototypeCreate();
	}
	return FListNodePrototype;
}
