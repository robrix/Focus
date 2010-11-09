// FMessage.h
// Created by Rob Rix on 2010-10-26
// Copyright 2010 Monochrome Industries

#ifndef F_MESSAGE
#define F_MESSAGE

#include <string.h>

typedef struct FMessage {
	struct FObject *context;
	struct FMessage *receiver;
	struct FSymbol *selector;
	struct FMessageNode *arguments;
} FMessage;

typedef struct FMessageNode {
	struct FMessage *message;
	struct FMessageNode *nextNode;
} FMessageNode;

FMessage *FMessageCreate(struct FObject *context, FMessage *receiver, struct FSymbol *selector, FMessageNode *arguments);
FMessage *FMessageCreateNullaryWithSubstring(struct FObject *context, FMessage *receiver, const char *string, size_t length);

FMessageNode *FMessageNodeCreate(FMessage *message);
void FMessageNodeSetNextNode(FMessageNode *self, FMessageNode *nextNode);
FMessageNode *FMessageNodeGetLastNode(FMessageNode *self);
size_t FMessageNodeGetNodeCount(FMessageNode *self);

#endif // F_MESSAGE
