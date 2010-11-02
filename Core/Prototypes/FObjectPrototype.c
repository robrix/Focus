// FObjectPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FObjectPrototype.h"

#include "../FSymbol.h"
#include <stdlib.h>

static FObject *FObjectPrototype = NULL;

FObject *FObjectPrototypeGetSelf(FObject *self, FSymbol *selector) {
	return self;
}

FObject *FObjectPrototypeGetPrototype(FObject *self, FSymbol *selector) {
	return self->prototype;
}

FObject *FObjectPrototypeClone(FObject *self, FSymbol *selector) {
	return FObjectCreate(self);
}


FObject *FObjectPrototypeCreate() {
	FObject *prototype = FObjectCreate(NULL);
	FObjectSetMethod(prototype, FSymbolCreateWithString("self"), (FMethod)FObjectPrototypeGetSelf);
	FObjectSetMethod(prototype, FSymbolCreateWithString("prototype"), (FMethod)FObjectPrototypeGetPrototype);
	FObjectSetMethod(prototype, FSymbolCreateWithString("new"), (FMethod)FObjectPrototypeClone);
	return prototype;
}

FObject *FObjectPrototypeGet() {
	if(!FObjectPrototype) {
		FObjectPrototype = FObjectPrototypeCreate();
	}
	return FObjectPrototype;
}
