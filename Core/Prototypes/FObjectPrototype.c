// FObjectPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FObjectPrototype.h"

#include "../FSymbol.h"
#include "../Prototypes/FFunctionPrototype.h"
#include <stdlib.h>

static FObject *FObjectPrototype = NULL;

FObject *FObjectPrototypeGetSelf(FObject *self, FObject *selector) {
	return self;
}

FObject *FObjectPrototypeGetPrototype(FObject *self, FObject *selector) {
	return FObjectGetPrototype(self);
}

FObject *FObjectPrototypeClone(FObject *self, FObject *selector) {
	return FObjectCreate(self);
}


FObject *FObjectPrototypeInitializeInEvaluator(FObject *prototype, FObject *evaluator) {
	FObjectSetMethod(prototype, FSymbolCreateWithString("self"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectPrototypeGetSelf));
	FObjectSetMethod(prototype, FSymbolCreateWithString("prototype"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectPrototypeGetPrototype));
	FObjectSetMethod(prototype, FSymbolCreateWithString("new"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectPrototypeClone));
	return prototype;
}

FObject *FObjectPrototypeGet() {
	if(!FObjectPrototype) {
		FObjectPrototype = FObjectCreate(NULL);
		FObjectPrototypeInitialize(FObjectPrototype);
		// FObjectSetMethod(FObjectPrototype, FSymbolCreateWithString("self"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectPrototypeGetSelf));
		// FObjectSetMethod(FObjectPrototype, FSymbolCreateWithString("prototype"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectPrototypeGetPrototype));
		// FObjectSetMethod(FObjectPrototype, FSymbolCreateWithString("new"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectPrototypeClone));
	}
	return FObjectPrototype;
}
