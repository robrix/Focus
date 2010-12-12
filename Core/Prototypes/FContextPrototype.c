// FContextPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FContextPrototype.h"

#include "../FSymbol.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

static FObject *FContextPrototype = NULL;

FObject *FContextPrototypeGetObjectPrototype(FObject *self, FObject *selector) {
	return FObjectPrototypeGet();
}

FObject *FContextPrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	// fixme: create a list of arguments
	FObjectSetMethod(prototype, FSymbolCreateWithString("Object"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextPrototypeGetObjectPrototype));
	return prototype;
}

FObject *FContextPrototypeGet() {
	if(!FContextPrototype) {
		FContextPrototype = FContextPrototypeCreate();
	}
	return FContextPrototype;
}
