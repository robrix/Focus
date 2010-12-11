// FFunctionPrototype.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FFunctionPrototype.h"

#include "../FSymbol.h"
#include "FObjectPrototype.h"
#include "../FFunction.h"
#include "../FObject+Protected.h"
#include <stdlib.h>

const size_t FFunctionArgumentNotFound = -1;
static FObject *FFunctionPrototype = NULL;

FObject *FFunctionNewWithArgumentsAndMessages(FObject *self, FObject *selector, FObject *arguments, FObject *messages) {
	FObject *function = FSend(FFunctionPrototypeGet(), new);
	FSend(function, arguments:, arguments);
	FSend(function, messages:, messages);
	return function;
}


FObject *FFunctionPrototypeGet() {
	if(!FFunctionPrototype) {
		FFunctionPrototype = FObjectCreate(FObjectPrototypeGet());
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("arguments"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("arguments:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("messages"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("messages:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
		
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("newWithArguments:messages:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FFunctionNewWithArgumentsAndMessages));
	}
	return FFunctionPrototype;
}


size_t FFunctionGetIndexOfArgument(FObject *function, FObject *symbol) {
	size_t index = 0, marker = FFunctionArgumentNotFound;
	FObject *node = FSend(function, arguments);
	while(node) {
		if(FSymbolIsEqual(FSend(node, object), symbol)) {
			marker = index;
			node = NULL;
		} else {
			node = FSend(node, next);
			index++;
		}
	}
	return marker;
}
