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


extern FObject *FObjectPrototypeGetSelf(FObject *self, FObject *selector);

FObject *FContextPrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetVariable(prototype, FSymbolCreateWithString("Allocator"), state.Allocator);
	FObjectSetVariable(prototype, FSymbolCreateWithString("Compiler"), state.Compiler);
	FObjectSetVariable(prototype, FSymbolCreateWithString("Evaluator"), state.Evaluator);
	FObjectSetVariable(prototype, FSymbolCreateWithString("Function"), state.Function);
	FObjectSetVariable(prototype, FSymbolCreateWithString("ListNode"), state.ListNode);
	FObjectSetVariable(prototype, FSymbolCreateWithString("Message"), state.Message);
	FObjectSetVariable(prototype, FSymbolCreateWithString("Object"), state.Object);
	FObjectSetMethod(prototype, FSymbolCreateWithString("Allocator"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Compiler"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Evaluator"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Function"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("ListNode"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Message"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Object"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	
	FObjectSetMethod(prototype, FSymbolCreateWithString("Context"), FEvaluatorBootstrapFunction((FImplementation)FObjectPrototypeGetSelf, state));
	
	return prototype;
}
