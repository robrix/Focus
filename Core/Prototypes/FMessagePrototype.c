// FMessagePrototype.c
// Created by Rob Rix on 2010-11-29
// Copyright 2010 Monochrome Industries

#include "FMessagePrototype.h"

#include "../FSymbol.h"
#include "../FFunction.h"
#include "FObjectPrototype.h"
#include "../FObject+Protected.h"
#include <stdlib.h>

static FObject *FMessagePrototype = NULL;

FObject *FMessagePrototypeGet() {
	if(!FMessagePrototype) {
		FMessagePrototype = FObjectCreate(FObjectPrototypeGet());
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("context"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("context:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("receiver"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("receiver:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("selector"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("selector:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("arguments"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
		FObjectSetMethod(FMessagePrototype, FSymbolCreateWithString("arguments:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectSetVariableAsAccessor));
	}
	return FMessagePrototype;
}


FObject *FMessageCreate(struct FObject *context, struct FObject *receiver, struct FSymbol *selector, FObject *arguments) {
	FObject *message = FSend(FMessagePrototypeGet(), new);
	if(context) FSend(message, context:, context);
	if(receiver) FSend(message, receiver:, receiver);
	if(selector) FSend(message, selector:, selector);
	if(arguments) FSend(message, arguments:, arguments);
	return message;
}

FObject *FMessageCreateNullaryWithSubstring(struct FObject *context, struct FObject *receiver, const char *string, size_t length) {
	FSymbol *selector = FSymbolCreateWithSubstring(string, length);
	return FMessageCreate(context, receiver, selector, NULL);
}