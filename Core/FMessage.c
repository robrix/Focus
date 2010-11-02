// FMessage.c
// Created by Rob Rix on 2010-10-26
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FMessage.h"
#include "FObject.h"
#include "FSymbol.h"

FMessage *FMessageCreate(FObject *context, FMessage *receiver, struct FSymbol *selector, FMessage *arguments) {
	FMessage *instance = FAllocatorAllocate(NULL, sizeof(FMessage));
	instance->context = context;
	instance->receiver = receiver;
	instance->selector = selector;
	instance->arguments = arguments;
	return instance;
}

FMessage *FMessageCreateNullaryWithSubstring(FObject *context, FMessage *receiver, const char *string, size_t length) {
	FSymbol *selector = FSymbolCreateWithSubstring(string, length);
	return FMessageCreate(context, receiver, selector, NULL);
}


// FObject *FMessageEvaluate(FMessage *self) {
// 	FObject *receiver = self->receiver ? FMessageEvaluate(self->receiver) : self->context;
// 	// return FSend(receiver, self->selector, )
// 	
// }
