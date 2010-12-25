// FEvaluator.c
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#include "FEvaluator.h"

FObject *FEvaluatorCreate() {
	FObject *Object = FObjectCreate(NULL);
	
	FObject *Allocator = FObjectCreate(Object);
	FObject *Compiler = FObjectCreate(Object);
	FObject *Context = FObjectCreate(Object);
	FObject *Function = FObjectCreate(Object);
	FObject *Symbol = FObjectCreate(Object);
	
	FObject *Evaluator = FObjectCreate(Object);
	
	FObjectSetVariable(Evaluator, FSymbolCreateWithPrototypeAndString(Symbol, "Allocator"), Allocator);
	FObjectSetVariable(Evaluator, FSymbolCreateWithPrototypeAndString(Symbol, "Compiler"), Compiler);
	FObjectSetVariable(Evaluator, FSymbolCreateWithPrototypeAndString(Symbol, "Context"), Context);
	FObjectSetVariable(Evaluator, FSymbolCreateWithPrototypeAndString(Symbol, "Function"), Function);
	FObjectSetVariable(Evaluator, FSymbolCreateWithPrototypeAndString(Symbol, "Object"), Object);
	FObjectSetVariable(Evaluator, FSymbolCreateWithPrototypeAndString(Symbol, "Symbol"), Symbol);
	
	FObjectPrototypeInitializeInEvaluator(Object, Evaluator); // internally this is relying on FObjectPrototypeGet. It needs to be parameterized with the evaluator.
	return Evaluator;
}


FObject *FEvaluatorEvaluateStringInContext(FEvaluator *evaluator, const char *string, FObject *context) {
	// parse the string
	// evaluate the resulting function within the provided context
	return NULL;
}
