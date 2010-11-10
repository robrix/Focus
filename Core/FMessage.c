// FMessage.c
// Created by Rob Rix on 2010-10-26
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FMessage.h"
#include "FObject.h"
#include "FSymbol.h"

FMessage *FMessageCreate(struct FObject *context, FMessage *receiver, struct FSymbol *selector, FMessageNode *arguments) {
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


FMessageNode *FMessageNodeCreate(FMessage *message) {
	FMessageNode *node = FAllocatorAllocate(NULL, sizeof(FMessageNode));
	node->message = message;
	return node;
}

void FMessageNodeSetNextNode(FMessageNode *self, FMessageNode *nextNode) {
	self->nextNode = nextNode;
}

FMessageNode *FMessageNodeGetLastNode(FMessageNode *self) {
	FMessageNode *node = self;
	while(node->nextNode != NULL) {
		node = node->nextNode;
	}
	return node;
}

size_t FMessageNodeGetNodeCount(FMessageNode *self) {
	size_t count = 0;
	do {
		count++;
	} while((self = self->nextNode));
	return count;
}
