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

FObject *FFunctionPrototypeGet() {
	if(!FFunctionPrototype) {
		FFunctionPrototype = FObjectCreate(FObjectPrototypeGet());
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("arguments"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectGetVariable));
		FObjectSetMethod(FFunctionPrototype, FSymbolCreateWithString("messages"), FFunctionCreateWithFunctionPointer(NULL, (FFunctionPointer)FObjectGetVariable));
	}
	return FFunctionPrototype;
}
