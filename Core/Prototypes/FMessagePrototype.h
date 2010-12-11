// FMessagePrototype.h
// Created by Rob Rix on 2010-11-29
// Copyright 2010 Monochrome Industries

#ifndef F_MESSAGE_PROTOTYPE
#define F_MESSAGE_PROTOTYPE

#include "../FObject.h"

FObject *FMessagePrototypeGet();

FObject *FMessageCreate(FObject *receiver, FObject *selector, FObject *arguments);
FObject *FMessageCreateNullaryWithSubstring(FObject *receiver, const char *string, size_t length);

#endif // F_MESSAGE_PROTOTYPE
