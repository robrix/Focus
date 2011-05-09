// FReference.c
// Created by Rob Rix on 2011-04-11
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"
#include "FReference.h"

typedef struct FReference {
	void **address;
	size_t offset;
	struct FReference *next;
} FReference;

struct FReference *FReferenceCreate(void **address, size_t offset) {
	FAssertPrecondition(address != NULL);
	FReference *reference = calloc(1, sizeof(FReference));
	reference->address = address;
	reference->offset = offset;
	return reference;
}

struct FReference *FReferenceCreateCopy(struct FReference *reference) {
	FAssertPrecondition(reference != NULL);
	return FReferenceCreate(reference->address, reference->offset);
}

void FReferenceDestroy(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	free(self);
}


void **FReferenceGetAddress(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	return self->address;
}

size_t FReferenceGetOffset(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	return self->offset;
}

struct FReference *FReferenceGetNextReference(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	return self->next;
}


struct FObject *FReferenceGetReferencedObject(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	return (struct FObject *)(*(self->address) - self->offset);
}

struct FObject *FReferenceSetReferencedObject(struct FReference *self, struct FObject *object) {
	return (struct FObject *)((*(self->address)) = ((void *)object + self->offset));
}


struct FReference *FReferenceListGetLastReference(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	return FReferenceGetNextReference(self)?
		FReferenceListGetLastReference(FReferenceGetNextReference(self))
	:	self;
}

struct FReference *FReferenceListAppendReference(struct FReference *self, struct FReference *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(other != NULL);
	FAssertPrecondition(self != other);
	return FReferenceListGetLastReference(self)->next = other;
}


void FReferenceListVisitReferences(struct FReference *self, FReferenceListReferenceVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(visitor != NULL);
	while(self != NULL) {
		visitor(self, context);
		self = FReferenceGetNextReference(self);
	}
}


void FReferenceListDestroyReference(struct FReference *reference, void *context) {
	FReferenceDestroy(reference);
}

void FReferenceListDestroy(struct FReference *self) {
	FAssertPrecondition(self != NULL);
	FReferenceListVisitReferences(self, FReferenceListDestroyReference, NULL);
}
