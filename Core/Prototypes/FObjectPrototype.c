// FObjectPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FObjectPrototype.h"

#include "../FSymbol.h"
#include <stdlib.h>

static FObject *FObjectPrototype = NULL;

FObject *FObjectPrototypeGetSelf(FObject *receiver, FSymbol *selector) {
	return receiver;
}

FObject *FObjectPrototypeClone(FObject *receiver, FSymbol *selector) {
	return FObjectCreate(receiver);
}


FObject *FObjectPrototypeCreate() {
	FObject *prototype = FObjectCreate(NULL);
	FObjectSetMethod(prototype, FSymbolCreate("self"), (FMethod)FObjectPrototypeGetSelf);
	FObjectSetMethod(prototype, FSymbolCreate("new"), (FMethod)FObjectPrototypeClone);
	return prototype;
}

FObject *FObjectPrototypeGet() {
	if(!FObjectPrototype) {
		FObjectPrototype = FObjectPrototypeCreate();
	}
	return FObjectPrototype;
}
