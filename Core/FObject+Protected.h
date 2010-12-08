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

extern struct FObject *FObjectGetVariable(struct FObject *self, struct FObject *selector);
extern struct FObject *FObjectGetVariableWithHash(struct FObject *self, struct FObject *selector, size_t hash); // for bootstrapping symbols
extern struct FObject *FObjectSetVariable(struct FObject *self, struct FObject *selector, struct FObject *variable);
extern struct FObject *FObjectSetVariableWithHash(struct FObject *self, struct FObject *selector, size_t hash, struct FObject *variable); // for bootstrapping symbols, which are themselves objects and thus have methods stored as objects indexed by… symbols!

extern struct FObject *FObjectSetVariableAsAccessor(struct FObject *self, struct FObject *selector, struct FObject *other);

#endif // F_OBJECT_PROTECTED
