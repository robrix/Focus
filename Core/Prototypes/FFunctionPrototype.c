// FFunctionPrototype.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FFunctionPrototype.h"

#include "../FSymbol.h"
#include "../FCompiler.h"
#include "FObjectPrototype.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "../Prototypes/FListNodePrototype.h"
#include "../FObject+Protected.h"
#include <stdlib.h>

const size_t FFunctionArgumentNotFound = -1;

FObject *FFunctionNewWithContextArgumentsMessages(FObject *self, FObject *selector, FObject *context, FObject *arguments, FObject *messages) {
	FObject *function = FSend(self, new);
	if(context)
		FSend(function, context:, context);
	if(arguments)
		FSend(function, arguments:, arguments);
	FSend(function, messages:, messages);
	return function;
}

FObject *FFunctionAcceptVisitor(FObject *self, FObject *selector, FObject *visitor) {
	return FSend(visitor, visitFunction:, self);
}


FObject *FFunctionPrototypeInitialize(FObject *prototype) {
	FObjectSetMethod(prototype, FSymbolCreateWithString("context"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("context:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
	FObjectSetMethod(prototype, FSymbolCreateWithString("arguments"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("arguments:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
	FObjectSetMethod(prototype, FSymbolCreateWithString("messages"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("messages:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("newWithContext:arguments:messages:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FFunctionNewWithContextArgumentsMessages));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("acceptVisitor:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FFunctionAcceptVisitor));

	return prototype;
}

FObject *FFunctionPrototypeGet() {
	static FObject *FFunctionPrototype = NULL;
	if(!FFunctionPrototype) {
		FFunctionPrototype = FObjectCreate(FObjectPrototypeGet());
		FFunctionPrototypeInitialize(FFunctionPrototype);
	}
	return FFunctionPrototype;
}


FObject *FFunctionCreateWithImplementation(FObject *arguments, FImplementation implementation) {
	FObject *function = FObjectCreate(FFunctionPrototypeGet());
	
	if(arguments) // fixme: arguments ought to be mandatory
		FObjectSetVariable(function, FSymbolCreateWithString("arguments"), arguments);
	
	FObjectSetVariable(function, FSymbolCreateWithString(" implementation"), (FObject *)implementation);
	return function;
}


FObject *FFunctionNoOp(FObject *self, FObject *selector) {
	return NULL;
}

FImplementation FFunctionGetImplementation(FObject *self) {
	if(!self) {
		return (FImplementation)FFunctionNoOp;
	}
	FImplementation implementation = (FImplementation)FObjectGetVariable(self, FSymbolCreateWithString(" implementation"));
	if(!implementation) {
		implementation = FCompilerCompileFunction(FSend(FSend(self, context), Compiler), self);
		FObjectSetVariable(self, FSymbolCreateWithString(" implementation"), (FObject *)implementation);
	}
	return implementation;
}


size_t FFunctionGetArity(FObject *self) {
	return FListNodeGetCount(FSend(self, arguments));
}


size_t FFunctionGetIndexOfArgument(FObject *function, FObject *symbol) {
	size_t index = 0, marker = FFunctionArgumentNotFound;
	FObject *node = FSend(function, arguments);
	while(node) {
		if(FSymbolIsEqual(FSend(node, object), symbol)) {
			marker = index;
			node = NULL;
		} else {
			node = FSend(node, next);
			index++;
		}
	}
	return marker;
}
