// FEvaluator.c
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#include "FEvaluator+Protected.h"
#include "FObject+Protected.h"
#include "Prototypes/FFunctionPrototype.h"
#include "FSymbol.h"


FObject *FEvaluatorBootstrapFunction(FImplementation implementation, FEvaluatorBootstrapState state) {
	return FFunctionCreateWithImplementation(state.Context, implementation);
}


extern FObject *FAllocatorPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FObjectPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FCompilerPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FContextPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FFunctionPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FListNodePrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);
extern FObject *FMessagePrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state);

FObject *FEvaluatorInitialize(FObject *Evaluator) {
	FSymbolInitialize();
	
	FObject *Object = FObjectCreate(NULL);
	FEvaluatorBootstrapState state = {
		.Object = Object,
		.Allocator = FObjectCreate(Object),
		.Compiler = FObjectCreate(Object),
		.Context = FObjectCreate(Object),
		.Function = FObjectCreate(Object),
		.ListNode = FObjectCreate(Object),
		.Message = FObjectCreate(Object),
		.Evaluator = Evaluator
	};
	Evaluator->prototype = Object;
	
	FObjectPrototypeBootstrap(Object, state);
	FContextPrototypeBootstrap(state.Context, state);
	FFunctionPrototypeBootstrap(state.Function, state);
	// FAllocatorPrototypeBootstrap(state.Allocator, state);
	FCompilerPrototypeBootstrap(state.Compiler, state);
	FMessagePrototypeBootstrap(state.Message, state);
	FListNodePrototypeBootstrap(state.ListNode, state);
	
	FObjectSetVariable(Evaluator, FSymbolCreateWithString("Context"), state.Context);
	FObjectSetMethod(Evaluator, FSymbolCreateWithString("Context"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	
	return Evaluator;
}

FObject *FEvaluatorCreate() {
	return FEvaluatorInitialize(FObjectCreate(NULL));
}

void FEvaluatorDestroy(FObject *evaluator) {
	
}


FObject *FEvaluatorEvaluateStringInContext(FObject *evaluator, const char *string, FObject *context) {
	// parse the string
	// evaluate the resulting function within the provided context
	return NULL;
}
