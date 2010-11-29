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

FObject *FListNodeNewWithObjectAndNextNode(FObject *self, FSymbol *selector, FObject *object, FObject *next) {
	FObject *instance = FSend(self, new);
	FObjectSetVariable(instance, FSymbolCreateWithString("object"), object);
	if(next) FObjectSetVariable(instance, FSymbolCreateWithString("next"), next);
	return instance;
}

FObject *FListNodeNewWithObject(FObject *self, FSymbol *selector, FObject *object) {
	return FListNodeNewWithObjectAndNextNode(self, selector, object, NULL);
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
	while(((self = FSend(self, next))) != NULL) {
		count++;
	}
	return count;
}
