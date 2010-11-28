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

FObject *FListNodeNewWithObject(FObject *self, FSymbol *selector, FObject *object) {
	FObject *instance = FSend(self, new);
	FObjectSetVariable(instance, FSymbolCreateWithString("object"), object);
	return instance;
}

FObject *FListNodeNewWithObjectAndNextNode(FObject *self, FSymbol *selector, FObject *object, FObject *next) {
	FObject *instance = FSend(self, new);
	FObjectSetVariable(instance, FSymbolCreateWithString("object"), object);
	FObjectSetVariable(instance, FSymbolCreateWithString("next"), next);
	return instance;
}


FObject *FListNodeGetLastNode(FObject *self, FSymbol *selector) {
	FObject *node = self, *next = NULL;
	while(((next = FSend(node, next))) != NULL) {
		node = next;
	}
	return node;
}


FObject *FListNodePrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("next"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("object"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectGetVariable));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithObject:"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FListNodeNewWithObject));
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithObject:nextNode:"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FListNodeNewWithObjectAndNextNode));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("last"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FListNodeGetLastNode));
	
	return prototype;
}

FObject *FListNodePrototypeGet() {
	if(!FListNodePrototype) {
		FListNodePrototype = FListNodePrototypeCreate();
	}
	return FListNodePrototype;
}
