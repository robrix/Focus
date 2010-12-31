// FEvaluator.h
// Created by Rob Rix on 2010-12-25
// Copyright 2010 Monochrome Industries

#ifndef F_EVALUATOR
#define F_EVALUATOR

#include "FObject.h"

FObject *FEvaluatorCreate();
#pragma message("Temporary function for iterative development.")
FObject *FEvaluatorGet();

FObject *FEvaluatorEvaluateStringInContext(FObject *evaluator, const char *string, FObject *context);

#endif // F_EVALUATOR
