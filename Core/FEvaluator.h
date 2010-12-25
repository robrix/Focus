// FEvaluator.h
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#ifndef F_EVALUATOR
#define F_EVALUATOR

#include "FObject.h"

/*
- allocator
- compiler
- each of the various prototypes (the GC roots, in other words)
*/

FObject *FEvaluatorCreate();

FObject *FEvaluatorEvaluateStringInContext(FEvaluator *evaluator, const char *string, FObject *context);

#endif // F_EVALUATOR
