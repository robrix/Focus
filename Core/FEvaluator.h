// FEvaluator.h
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#ifndef F_EVALUATOR
#define F_EVALUATOR

#include "FObject.h"

FObject *FEvaluatorCreate();
void FEvaluatorDestroy(FObject *evaluator);

FObject *FEvaluatorEvaluateStringInContext(FObject *evaluator, const char *string, FObject *context);

#endif // F_EVALUATOR
