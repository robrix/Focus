// FEvaluator.c
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#include "FEvaluator+Protected.h"
#include "FObject+Protected.h"
#include "Prototypes/FFunctionPrototype.h"
#include "FSymbol.h"
#include "FCompiler.h"

extern FObject *FObjectPrototypeGetSelf(FObject *self, FObject *selector);
extern FObject *FObjectPrototypeGetPrototype(FObject *self, FObject *selector);
extern FObject *FObjectPrototypeClone(FObject *self, FObject *selector);


FObject *FEvaluatorBootstrapSymbol(const char *string, FEvaluatorBootstrapState state) {
	return FSymbolCreateWithPrototypeAndString(state.Symbol, string);
}

FObject *FEvaluatorBootstrapFunction(FImplementation implementation, FEvaluatorBootstrapState state) {
	FObject *function = FObjectCreate(state.Function);
	FObjectSetVariable(function, FEvaluatorBootstrapSymbol(" implementation", state), (FObject *)implementation);
	return function;
}


extern FObject *FObjectPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FContextPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FFunctionPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);

FObject *FEvaluatorInitialize(FObject *Evaluator) {
	FEvaluatorBootstrapState state = {
		.Object = FObjectCreate(NULL),
		.Allocator = FObjectCreate(state.Object),
		.Compiler = FObjectCreate(state.Object),
		.Context = FObjectCreate(state.Object),
		.Function = FObjectCreate(state.Object),
		.Symbol = FObjectCreate(state.Object),
		.Evaluator = Evaluator
	};
	Evaluator->prototype = state.Object;
	
	FObjectPrototypeBootstrap(state.Object, state);
	FContextPrototypeBootstrap(state.Context, state);
	FFunctionPrototypeBootstrap(state.Function, state);
	// FAllocatorPrototypeBootstrap(state.Allocator, state);
	// FSymbolPrototypeBootstrap(state.Symbol, state);
	// FCompilerPrototypeBootstrap(state.Compiler, state);
	
	// FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Object", state), state.Object);
	// FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Function", state), state.Function);
	// FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Symbol", state), state.Symbol);
	// FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Allocator", state), state.Allocator);
	// FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Compiler", state), state.Compiler);
	FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Context", state), state.Context);
	
	// FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Object", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	// FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Function", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	// FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Symbol", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	// FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Allocator", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	// FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Compiler", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Context", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	
	FCompilerInitialize(state.Compiler);
	return Evaluator;
}


FObject *FEvaluatorGet() {
	static FObject *FGlobalEvaluator = NULL;
	if(!FGlobalEvaluator) {
		FGlobalEvaluator = FObjectCreate(NULL);
		FEvaluatorInitialize(FGlobalEvaluator);
	}
	return FGlobalEvaluator;
}


FObject *FEvaluatorEvaluateStringInContext(FObject *evaluator, const char *string, FObject *context) {
	// parse the string
	// evaluate the resulting function within the provided context
	return NULL;
}
