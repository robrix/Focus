// FObjectPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FObjectPrototype.h"

#include "../FEvaluator+Protected.h"

FObject *FObjectPrototypeGetSelf(FObject *self, FObject *selector) {
	return self;
}

FObject *FObjectPrototypeGetPrototype(FObject *self, FObject *selector) {
	return FObjectGetPrototype(self);
}

FObject *FObjectPrototypeClone(FObject *self, FObject *selector) {
	return FObjectCreate(self);
}


FObject *FObjectPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("self", state), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetSelf, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("prototype", state), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetPrototype, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("new", state), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeClone, state));
	return prototype;
}
