// FocusDriver.c
// Created by Rob Rix on 2011-01-01
// Copyright 2011 Monochrome Industries

#include "FEvaluator.h"

int main(int argc, const char *argv[]) {
	FObject *evaluator = FEvaluatorCreate();
	// 0 args -> evaluate stdin
	// 1 arg -> evaluate file, or stdin if it’s -
	FEvaluatorEvaluateStringInContext(evaluator, "Object", FSend(evaluator, Context));
	return 0;
}
