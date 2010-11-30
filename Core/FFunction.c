// FFunction.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FSymbol.h"
#include "FObject+Protected.h"
#include "FObject.h"
#include "FAllocator.h"
#include "FFunction.h"
#include "Prototypes/FFunctionPrototype.h"
#include "Prototypes/FListNodePrototype.h"

struct FFunction {
	FObject super;
	FFunctionPointer functionPointer;
};


FFunction *FFunctionCreateWithFunctionPointer(FObject *arguments, FFunctionPointer functionPointer) {
	FFunction *function = FAllocatorAllocate(NULL, sizeof(FFunction));
	
	function->super.prototype = FFunctionPrototypeGet();
	
	if(arguments) // fixme: arguments ought to be mandatory
		FObjectSetVariable((FObject *)function, FSymbolCreateWithString("arguments"), arguments);
	
	function->functionPointer = functionPointer;
	return function;
}


FObject *FFunctionNoOp(FObject *self, struct FSymbol *selector) {
	return NULL;
}

FFunctionPointer FFunctionGetFunctionPointer(FFunction *self) {
	if(!self) {
		return (FFunctionPointer)FFunctionNoOp;
	}
	if(!self->functionPointer) {
		// compile it
	}
	return self->functionPointer;
}


size_t FFunctionGetArity(FFunction *self) {
	return FListNodeGetCount(FSend(self, arguments));
}
