// FListNodePrototype.c
// Created by Rob Rix on 2010-11-21
// Copyright 2010 Monochrome Industries

#include "FListNodePrototype.h"

#include "../FSymbol.h"
#include "../FEvaluator+Protected.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "../FObject+Protected.h"
#include "FObjectPrototype.h"
#include <stdlib.h>
#include <stdarg.h>


FObject *FListNodeNewWithObjectAndNextNode(FObject *self, FObject *selector, FObject *object, FObject *next) {
	FObject *instance = FSend(self, new);
	FObjectSetVariable(instance, FSymbolCreateWithString("object"), object);
	if(next) FObjectSetVariable(instance, FSymbolCreateWithString("next"), next);
	return instance;
}

FObject *FListNodeNewWithObject(FObject *self, FObject *selector, FObject *object) {
	return FListNodeNewWithObjectAndNextNode(self, selector, object, NULL);
}


FObject *FListNodeGetLastNode(FObject *self, FObject *selector) {
	FObject *node = self, *next = NULL;
	while(((next = FSend(node, next))) != NULL) {
		node = next;
	}
	return node;
}


FObject *FListNodePrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetMethod(prototype, FSymbolCreateWithString("next"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("next:"), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("object"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("object:"), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithObject:"), FEvaluatorBootstrapFunction((FImplementation)FListNodeNewWithObject, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithObject:nextNode:"), FEvaluatorBootstrapFunction((FImplementation)FListNodeNewWithObjectAndNextNode, state));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("last"), FEvaluatorBootstrapFunction((FImplementation)FListNodeGetLastNode, state));
	
	return prototype;
}


size_t FListNodeGetCount(FObject *self) {
	size_t count = 0;
	while(self) {
		count++;
		self = FSend(self, next);
	}
	return count;
}
