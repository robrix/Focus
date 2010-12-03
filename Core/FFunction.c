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

FObject *FFunctionCreateWithImplementation(FObject *arguments, FImplementation implementation) {
	FObject *function = FObjectCreate(FFunctionPrototypeGet());
	
	if(arguments) // fixme: arguments ought to be mandatory
		FObjectSetVariable(function, FSymbolCreateWithString("arguments"), arguments);
	
	FObjectSetVariable(function, FSymbolCreateWithString(" implementation"), (FObject *)implementation);
	return function;
}


FObject *FFunctionNoOp(FObject *self, struct FSymbol *selector) {
	return NULL;
}

FImplementation FFunctionGetImplementation(FObject *self) {
	if(!self) {
		return (FImplementation)FFunctionNoOp;
	}
	FImplementation implementation = (FImplementation)FObjectGetVariable(self, FSymbolCreateWithString(" implementation"));
	if(!implementation) {
		// compile it
	}
	return implementation;
}


size_t FFunctionGetArity(FObject *self) {
	return FListNodeGetCount(FSend(self, arguments));
}
