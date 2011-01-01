// FEvaluator.c
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#include "FEvaluator+Protected.h"
#include "FObject+Protected.h"
#include "Prototypes/FFunctionPrototype.h"
#include "FSymbol.h"


FObject *FEvaluatorBootstrapSymbol(const char *string, FEvaluatorBootstrapState state) {
	return FSymbolCreateWithPrototypeAndString(state.Context, string);
}

FObject *FEvaluatorBootstrapFunction(FImplementation implementation, FEvaluatorBootstrapState state) {
	return FFunctionCreateWithImplementation(state.Context, implementation);
	// FObject *function = FObjectCreate(state.Function);
	// FObjectSetVariable(function, FEvaluatorBootstrapSymbol(" implementation", state), (FObject *)implementation);
	// return function;
}


extern FObject *FAllocatorPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FObjectPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FCompilerPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FContextPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FFunctionPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FListNodePrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FMessagePrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FSymbolPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);

FObject *FEvaluatorInitialize(FObject *Evaluator) {
	FObject *Object = FObjectCreate(NULL);
	FEvaluatorBootstrapState state = {
		.Object = Object,
		.Allocator = FObjectCreate(Object),
		.Compiler = FObjectCreate(Object),
		.Context = FObjectCreate(Object),
		.Function = FObjectCreate(Object),
		.ListNode = FObjectCreate(Object),
		.Message = FObjectCreate(Object),
		.Symbol = FObjectCreate(Object),
		.Evaluator = Evaluator
	};
	Evaluator->prototype = Object;
	
	FObjectPrototypeBootstrap(Object, state);
	FContextPrototypeBootstrap(state.Context, state);
	FFunctionPrototypeBootstrap(state.Function, state);
	// FAllocatorPrototypeBootstrap(state.Allocator, state);
	// FSymbolPrototypeBootstrap(state.Symbol, state);
	FCompilerPrototypeBootstrap(state.Compiler, state);
	FMessagePrototypeBootstrap(state.Message, state);
	FListNodePrototypeBootstrap(state.ListNode, state);
	
	FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Context", state), state.Context);
	FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Context", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	
	return Evaluator;
}

FObject *FEvaluatorCreate() {
	return FEvaluatorInitialize(FObjectCreate(NULL));
}


FObject *FEvaluatorEvaluateStringInContext(FObject *evaluator, const char *string, FObject *context) {
	// parse the string
	// evaluate the resulting function within the provided context
	return NULL;
}
