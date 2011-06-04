// FReference.h
// Created by Rob Rix on 2011-04-11
// Copyright 2011 Monochrome Industries

#ifndef F_REFERENCE
#define F_REFERENCE

#include <stdlib.h>

struct FReference *FReferenceCreate(void **address, size_t offset);
struct FReference *FReferenceCreateCopy(struct FReference *reference);
void FReferenceDestroy(struct FReference *self);

void **FReferenceGetAddress(struct FReference *self);
size_t FReferenceGetOffset(struct FReference *self);
struct FReference *FReferenceGetNextReference(struct FReference *self);

struct FObject *FReferenceGetReferencedObject(struct FReference *self);
struct FObject *FReferenceSetReferencedObject(struct FReference *self, struct FObject *object);

struct FReference *FReferenceListGetLastReference(struct FReference *self);
struct FReference *FReferenceListAppendReference(struct FReference *self, struct FReference *other);

typedef void (*FReferenceListReferenceVisitor)(struct FReference *reference, void *context);
void FReferenceListVisitReferences(struct FReference *self, FReferenceListReferenceVisitor visitor, void *context);

void FReferenceListSetReferencedObject(struct FReference *self, struct FObject *object);

void FReferenceListDestroy(struct FReference *self);

#endif // F_REFERENCE
