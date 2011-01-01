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
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("next", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("next:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("object", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("object:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("newWithObject:", state), FEvaluatorBootstrapFunction((FImplementation)FListNodeNewWithObject, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("newWithObject:nextNode:", state), FEvaluatorBootstrapFunction((FImplementation)FListNodeNewWithObjectAndNextNode, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("last", state), FEvaluatorBootstrapFunction((FImplementation)FListNodeGetLastNode, state));
	
	return prototype;
}


FObject *FListNodeCreateWithObjects(FObject *context, FObject *object, ...) {
	va_list arguments;
	FObject *root = NULL, *node = NULL;
	va_start(arguments, object);
	root = node = FSend(FSend(context, ListNode), newWithObject:, object);
	while((object = va_arg(arguments, FObject *))) {
		FObject *next = FSend(FSend(context, ListNode), newWithObject:, object);
		FObjectSetVariable(node, FSymbolCreateWithString("next"), next);
		node = next;
	}
	va_end(arguments);
	return root;
}


size_t FListNodeGetCount(FObject *self) {
	size_t count = 0;
	while(self) {
		count++;
		self = FSend(self, next);
	}
	return count;
}
