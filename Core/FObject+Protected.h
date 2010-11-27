// FObject+Protected.h
// Created by Rob Rix on 2010-11-11
// Copyright 2010 Monochrome Industries

#ifndef F_OBJECT_PROTECTED
#define F_OBJECT_PROTECTED

struct FObject {
	struct FObject *prototype;
	struct FHashTable *variables;
	struct FHashTable *methods;
};

extern FObject *FObjectGetVariable(FObject *self, struct FSymbol *selector);
extern FObject *FObjectSetVariable(FObject *self, struct FSymbol *selector, FObject *other);

#endif // F_OBJECT_PROTECTED
