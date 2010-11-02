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
	struct FMessage *arguments;
	struct FMessage *nextMessage;
} FMessage;

FMessage *FMessageCreate(struct FObject *context, FMessage *receiver, struct FSymbol *selector, FMessage *arguments);
FMessage *FMessageCreateNullaryWithSubstring(struct FObject *context, FMessage *receiver, const char *string, size_t length);

// FObject *FMessageEvaluate(FMessage *self);

#endif // F_MESSAGE
