// FContextPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FContextPrototype.h"

#include "../FSymbol.h"
#include "../FObject+Protected.h"
#include "../FCompiler.h"
#include "../FEvaluator.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

FObject *FContextGetEvaluator(FObject *self) {
	FObject *evaluator = FSend(self, Evaluator);
	return evaluator ?: FContextGetEvaluator(FObjectGetPrototype(self));
}

FObject *FContextGetEvaluatorVariable(FObject *self, FObject *selector) {
	return FSendMessage(FContextGetEvaluator(self), selector);
}

// FObject *FContextPrototypeCreateWithEvaluator(FObject *evaluator) {
// 	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
// 	// fixme: create a list of arguments
// 	FObjectSetVariable(prototype, FSymbolCreateWithString("Evaluator"), evaluator);
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Evaluator"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
// 	
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Allocator"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Compiler"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Context"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Function"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Object"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
// 	FObjectSetMethod(prototype, FSymbolCreateWithString("Symbol"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextGetEvaluatorVariable));
// 		
// 	return prototype;
// }
// 
// FObject *FContextPrototypeCreate() {
// 	return FContextPrototypeCreateWithEvaluator(FEvaluatorGet());
// }

FObject *FContextPrototypeGet() {
	// static FObject *FContextPrototype = NULL;
	// if(!FContextPrototype) {
	// 	FContextPrototype = FContextPrototypeCreate();
	// }
	return FSend(FEvaluatorGet(), Context);
}
