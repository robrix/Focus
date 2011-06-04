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

FObject *FFunctionNewWithContextArgumentsMessages(FObject *self, FSymbol *selector, FObject *context, FObject *arguments, FObject *messages) {
	FObject *function = FSend(self, new);
	if(context)
		FSend(function, context:, context);
	if(arguments)
		FSend(function, arguments:, arguments);
	FSend(function, messages:, messages);
	return function;
}

FObject *FFunctionAcceptVisitor(FObject *self, FSymbol *selector, FObject *visitor) {
	return FSend(visitor, visitFunction:, self);
}


FObject *FFunctionPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetVariable(prototype, FSymbolCreateWithString("context"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetVariable(prototype, FSymbolCreateWithString("context:"), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetVariable(prototype, FSymbolCreateWithString("arguments"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetVariable(prototype, FSymbolCreateWithString("arguments:"), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetVariable(prototype, FSymbolCreateWithString("messages"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetVariable(prototype, FSymbolCreateWithString("messages:"), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	
	FObjectSetVariable(prototype, FSymbolCreateWithString("newWithContext:arguments:messages:"), FEvaluatorBootstrapFunction((FImplementation)FFunctionNewWithContextArgumentsMessages, state));
	
	FObjectSetVariable(prototype, FSymbolCreateWithString("acceptVisitor:"), FEvaluatorBootstrapFunction((FImplementation)FFunctionAcceptVisitor, state));

	return prototype;
}


FObject *FFunctionCreateWithImplementation(FObject *context, FImplementation implementation) {
	FObject *function = FObjectCreate(FSend(context, Function));
	FObjectSetVariable(function, FSymbolCreateWithString(" implementation"), (FObject *)implementation);
	return function;
}


FObject *FFunctionNoOp(FObject *self, FSymbol *selector) {
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
