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

extern struct FObject *FObjectGetVariable(struct FObject *self, struct FSymbol *selector);
extern struct FObject *FObjectSetVariable(struct FObject *self, struct FSymbol *selector, struct FObject *other);

extern struct FObject *FObjectSetVariableAsAccessor(struct FObject *self, struct FSymbol *selector, struct FObject *other);

#endif // F_OBJECT_PROTECTED
