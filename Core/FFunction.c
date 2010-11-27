// FFunction.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "FObject+Protected.h"
#include "FObject.h"
#include "FAllocator.h"
#include "FFunction.h"

struct FFunction {
	FObject super;
	FFunctionPointer functionPointer;
};


FFunction *FFunctionCreateWithFunctionPointer(FObject *arguments, FFunctionPointer functionPointer) {
	FFunction *function = FAllocatorAllocate(NULL, sizeof(FFunction));
	// set the prototype
	// set the arguments
	function->functionPointer = functionPointer;
	return function;
}


FFunctionPointer FFunctionGetFunctionPointer(FFunction *self) {
	if(!self->functionPointer) {
		// compile it
	}
	return self->functionPointer;
}
