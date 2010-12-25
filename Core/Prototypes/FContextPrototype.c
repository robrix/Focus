// FContextPrototype.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "FContextPrototype.h"

#include "../FSymbol.h"
#include "../FCompiler.h"
#include "../Prototypes/FFunctionPrototype.h"
#include "FObjectPrototype.h"
#include <stdlib.h>

static FObject *FContextPrototype = NULL;

FObject *FContextPrototypeGetObjectPrototype(FObject *self, FObject *selector) {
	return FObjectPrototypeGet();
}

FObject *FContextPrototypeGetCompilerPrototype(FObject *self, FObject *selector) {
	static FObject *FCompilerPrototype = NULL;
	if(!FCompilerPrototype) {
		FCompilerPrototype = FCompilerCreate();
	}
	return FCompilerPrototype;
}

FObject *FContextPrototypeCreate() {
	FObject *prototype = FObjectCreate(FObjectPrototypeGet());
	// fixme: create a list of arguments
	FObjectSetMethod(prototype, FSymbolCreateWithString("Object"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextPrototypeGetObjectPrototype));
	FObjectSetMethod(prototype, FSymbolCreateWithString("Compiler"), FFunctionCreateWithImplementation(NULL, (FImplementation)FContextPrototypeGetCompilerPrototype));
	return prototype;
}

FObject *FContextPrototypeGet() {
	if(!FContextPrototype) {
		FContextPrototype = FContextPrototypeCreate();
	}
	return FContextPrototype;
}
