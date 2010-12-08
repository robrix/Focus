// FFunctionPrototype.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FFunctionPrototype.h"

#include "../FSymbol.h"
#include "FObjectPrototype.h"
#include "../FFunction.h"
#include "../FObject+Protected.h"
#include <stdlib.h>

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
