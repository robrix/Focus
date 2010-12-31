// FEvaluator.c
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#include "FEvaluator+Protected.h"
#include "FObject+Protected.h"
#include "Prototypes/FFunctionPrototype.h"
#include "FSymbol.h"


FObject *FEvaluatorBootstrapSymbol(const char *string, FEvaluatorBootstrapState state) {
	return FSymbolCreateWithPrototypeAndString(state.Symbol, string);
}

FObject *FEvaluatorBootstrapFunction(FImplementation implementation, FEvaluatorBootstrapState state) {
	FObject *function = FObjectCreate(state.Function);
	FObjectSetVariable(function, FEvaluatorBootstrapSymbol(" implementation", state), (FObject *)implementation);
	return function;
}


extern FObject *FObjectPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FCompilerPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
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
		.ListNode = FObjectCreate(state.Object),
		.Message = FObjectCreate(state.Object),
		.Evaluator = Evaluator
	};
	Evaluator->prototype = state.Object;
	
	FObjectPrototypeBootstrap(state.Object, state);
	FContextPrototypeBootstrap(state.Context, state);
	FFunctionPrototypeBootstrap(state.Function, state);
	// FAllocatorPrototypeBootstrap(state.Allocator, state);
	// FSymbolPrototypeBootstrap(state.Symbol, state);
	FCompilerPrototypeBootstrap(state.Compiler, state);
	// FMessagePrototypeBootstrap(state.Message, state);
	// FListNodePrototypeBootstrap(state.ListNode, state);
	
	FObjectSetVariable(Evaluator, FEvaluatorBootstrapSymbol("Context", state), state.Context);
	FObjectSetMethod(Evaluator, FEvaluatorBootstrapSymbol("Context", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	
	return Evaluator;
}

FObject *FEvaluatorCreate() {
	return FEvaluatorInitialize(FObjectCreate(NULL));
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
