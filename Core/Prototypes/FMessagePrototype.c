// FMessagePrototype.c
// Created by Rob Rix on 2010-11-29
// Copyright 2010 Monochrome Industries

#include "FMessagePrototype.h"

#include "../FSymbol.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "../Prototypes/FListNodePrototype.h"
#include "FObjectPrototype.h"
#include "../FObject+Protected.h"
#include "../FEvaluator+Protected.h"
#include <stdlib.h>


FObject *FMessageNewWithReceiverSelectorArguments(FObject *self, FObject *_cmd, FObject *receiver, FObject *selector, FObject *arguments) {
	FObject *instance = FSend(self, new);
	if(receiver) FSend(instance, receiver:, receiver);
	if(selector) FSend(instance, selector:, selector);
	if(arguments) FSend(instance, arguments:, arguments);
	return instance;
}


FObject *FMessageAcceptVisitor(FObject *self, FObject *selector, FObject *visitor) {
	FObject *receiver = FSend(self, receiver) ? FSend(FSend(self, receiver), acceptVisitor:, visitor) : NULL;
	FObject *node = FSend(self, arguments);
	size_t count = FListNodeGetCount(node), index = 0;
	FObject *arguments[count];
	while(node) {
		arguments[index++] = FSend(FSend(node, object), acceptVisitor:, visitor);
		node = FSend(node, next);
	}
	return FSend(visitor, visitMessage:withVisitedReceiver:visitedArguments:, self, receiver, arguments);
}


FObject *FMessagePrototypeBootstrap(FObject *prototype, FEvaluatorBootstrapState state) {
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("receiver", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("receiver:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("selector", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("selector:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("arguments", state), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("arguments:", state), FEvaluatorBootstrapFunction((FImplementation)FObjectSetVariableAsAccessor, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("newWithReceiver:selector:arguments:", state), FEvaluatorBootstrapFunction((FImplementation)FMessageNewWithReceiverSelectorArguments, state));
	
	FObjectSetMethod(prototype, FEvaluatorBootstrapSymbol("acceptVisitor:", state), FEvaluatorBootstrapFunction((FImplementation)FMessageAcceptVisitor, state));
	
	return prototype;
}
