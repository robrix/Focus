// FEvaluator+Protected.h
// Created by Rob Rix on 2010-12-28
// Copyright 2010 Monochrome Industries

#ifndef F_EVALUATOR_PROTECTED
#define F_EVALUATOR_PROTECTED

#include "FEvaluator.h"

typedef struct FEvaluatorBootstrapState {
	FObject *Object;
	FObject *Symbol;
	FObject *Function;
	FObject *Compiler;
	FObject *Allocator;
	FObject *Context;
	FObject *Evaluator;
} FEvaluatorBootstrapState;

extern FObject *FEvaluatorBootstrapSymbol(const char *string, FEvaluatorBootstrapState state);
extern FObject *FEvaluatorBootstrapFunction(FImplementation implementation, FEvaluatorBootstrapState state);

#endif // F_EVALUATOR_PROTECTED
