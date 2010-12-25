// FContextPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FContextPrototype.h"

#include "../FSymbol.h"
#include "../FCompiler.h"
#include "../FEvaluator.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

static FObject *FContextPrototype = NULL;

FObject *FContextGetEvaluator(FObject *self) {
	return FSend(FObjectGetPrototype(self) ?: self, Evaluator);
}

FObject *FContextGetEvaluatorVariable(FObject *self, FObject *selector) {
	return FSendMessage(FContextGetEvaluator(self), selector);
}

FObject *FContextPrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	// fixme: create a list of arguments
	FObjectSetMethod(prototype, FSymbolCreateWithString("Evaluator"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("Allocator"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Compiler"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Context"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Function"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Object"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Symbol"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
	
	
	return prototype;
}

FObject *FContextPrototypeGet() {
	if(!FContextPrototype) {
		FContextPrototype = FContextPrototypeCreate();
	}
	return FContextPrototype;
}
