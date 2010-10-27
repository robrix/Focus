// FMessage.c
// Created by Rob Rix on 2010-10-26
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FMessage.h"
#include "FObject.h"

FMessage *FMessageCreate(FMessage *receiver, struct FSymbol *selector, struct FArray *arguments) {
	FMessage *instance = FAllocatorAllocate(NULL, sizeof(FMessage));
	instance->receiver = receiver;
	instance->selector = selector;
	instance->arguments = arguments;
	return instance;
}


// FObject *FMessageEvaluate(FMessage *self) {
// 	FObject *receiver = self->receiver ? FMessageEvaluate(self->receiver) : self->context;
// 	// return FSend(receiver, self->selector, )
// 	
// }
