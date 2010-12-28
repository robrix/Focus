// FEvaluator+Protected.h
// Created by Rob Rix on 2010-12-28
// Copyright 2010 Monochrome Industries

#include "FEvaluator.h"

#ifndef F_EVALUATOR_PROTECTED
#define F_EVALUATOR_PROTECTED

typedef struct FEvaluatorBootstrapState {
	FObject *Object;
	FObject *Symbol;
	FObject *Function;
	FObject *Compiler;
	FObject *Allocator;
	FObject *Context;
	FObject *Evaluator;
} FEvaluatorBootstrapState;

#endif // F_EVALUATOR_PROTECTED
