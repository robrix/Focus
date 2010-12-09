// FListNodePrototype.c
// Created by Rob Rix on 2010-11-21
// Copyright 2010 Monochrome Industries

#include "FListNodePrototype.h"

#include "../FSymbol.h"
#include "../FFunction.h"
#include "../FObject+Protected.h"
#include "FObjectPrototype.h"
#include <stdlib.h>
#include <stdarg.h>

static FObject *FListNodePrototype = NULL;

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


FObject *FListNodePrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("next"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("next:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
	FObjectSetMethod(prototype, FSymbolCreateWithString("object"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("object:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithObject:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FListNodeNewWithObject));
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithObject:nextNode:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FListNodeNewWithObjectAndNextNode));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("last"), FFunctionCreateWithImplementation(NULL, (FImplementation)FListNodeGetLastNode));
	
	return prototype;
}

FObject *FListNodePrototypeGet() {
	if(!FListNodePrototype) {
		FListNodePrototype = FListNodePrototypeCreate();
	}
	return FListNodePrototype;
}


FObject *FListNodeCreateWithObject(FObject *object) {
	return FSend(FListNodePrototypeGet(), newWithObject:, object);
}

FObject *FListNodeCreateWithObjects(FObject *object, ...) {
	va_list arguments;
	FObject *root = NULL, *node = NULL;
	va_start(arguments, object);
	root = node = FSend(FListNodePrototypeGet(), newWithObject:, object);
	while((object = va_arg(arguments, FObject *))) {
		FObject *next = FSend(FListNodePrototypeGet(), newWithObject:, object);
		FObjectSetVariable(node, FSymbolCreateWithString("next"), next);
		node = next;
	}
	va_end(arguments);
	return root;
}

// FObject *FListNodeCreateWithObjectsAndCount(FObject **objects, size_t count) {
// 	
// }


size_t FListNodeGetCount(FObject *self) {
	size_t count = 0;
	while(self) {
		count++;
		self = FSend(self, next);
	}
	return count;
}
