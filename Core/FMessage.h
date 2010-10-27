// FMessage.h
// Created by Rob Rix on 2010-10-26
// Copyright 2010 Monochrome Industries

#ifndef F_MESSAGE
#define F_MESSAGE

typedef struct FMessage {
	struct FObject *context;
	struct FMessage *receiver;
	struct FSymbol *selector;
	struct FArray *arguments;
} FMessage;

FMessage *FMessageCreate(FMessage *receiver, struct FSymbol *selector, struct FArray *arguments);

// FObject *FMessageEvaluate(FMessage *self);

#endif // F_MESSAGE
