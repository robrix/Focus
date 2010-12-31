// FContextPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FContextPrototype.h"

#include "../FSymbol.h"
#include "../FObject+Protected.h"
#include "../FCompiler.h"
#include "../FEvaluator+Protected.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

FObject *FContextGetEvaluator(FObject *self) {
	FObject *evaluator = FSend(self, Evaluator);
	return evaluator ?: FContextGetEvaluator(FObjectGetPrototype(self));
}

extern FObject *FObjectPrototypeGetSelf(FObject *self, FObject *selector);
extern FObject *FObjectPrototypeGetPrototype(FObject *self, FObject *selector);


FObject *FContextPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetVariable(prototype, FEvaluatorBootstrapSymbol("Allocator", state), state.Allocator);
	FObjectSetVariable(prototype, FEvaluatorBootstrapSymbol("Compiler", state), state.Compiler);
	FObjectSetVariable(prototype, FEvaluatorBootstrapSymbol("Evaluator", state), state.Evaluator);
	FObjectSetVariable(prototype, FEvaluatorBootstrapSymbol("Function", state), state.Function);
	// FObjectSetVariable(prototype, FEvaluatorBootstrapSymbol("Object", state), state.Object);
	FObjectSetVariable(prototype, FEvaluatorBootstrapSymbol("Symbol", state), state.Symbol);
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Allocator", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Compiler", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Evaluator", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Function", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	// FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Object", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Symbol", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Context", state), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetSelf, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("Object", state), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetPrototype, state));
	
	return prototype;
}

FObject *FContextPrototypeGet() {
	return FSend(FEvaluatorGet(), Context);
}
