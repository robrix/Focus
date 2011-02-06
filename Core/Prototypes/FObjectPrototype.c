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
	FObjectSetMethod(prototype, FSymbolCreateWithString("self"), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetSelf, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("prototype"), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetPrototype, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("new"), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeClone, state));
	return prototype;
}
