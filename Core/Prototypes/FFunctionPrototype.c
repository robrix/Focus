// FFunctionPrototype.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FFunctionPrototype.h"

#include "../FSymbol.h"
#include "../FCompiler.h"
#include "../FEvaluator+Protected.h"
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


FObject *FFunctionPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("context", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("context:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("arguments", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("arguments:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("messages", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("messages:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("newWithContext:arguments:messages:", state), FEvaluatorBootstrapFunction((FImplementation)FFunctionNewWithContextArgumentsMessages, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("acceptVisitor:", state), FEvaluatorBootstrapFunction((FImplementation)FFunctionAcceptVisitor, state));

	return prototype;
}

FObject *FFunctionPrototypeGet() {
	return FSend(FSend(FEvaluatorGet(), Context), Function);
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
