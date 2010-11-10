// FFunctionPrototype.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FFunctionPrototype.h"

#include "../FSymbol.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

static FObject *FFunctionPrototype = NULL;

FObject *FFunctionPrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	// FObjectSetSlot(prototype, FSymbolCreateWithString("Object"), (FMethod)FContextPrototypeGetObjectPrototype);
	return prototype;
}

FObject *FFunctionPrototypeGet() {
	if(!FFunctionPrototype) {
		FFunctionPrototype = FFunctionPrototypeCreate();
	}
	return FFunctionPrototype;
}
