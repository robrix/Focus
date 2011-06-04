// FFrame.c
// Created by Rob Rix on 2011-04-11
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"
#include "FObject.h"
#include "FFrame.h"
#include "FReference.h"

#include <stdlib.h>

typedef struct FFrame {
	const char *name;
	struct FReference *references;
	struct FFrame *prev;
} FFrame;


struct FFrame *FFrameCreate(const char *name) {
	FAssertPrecondition(name != NULL);
	FFrame *frame = calloc(1, sizeof(FFrame));
	frame->name = name;
	return frame;
}

void FFrameDestroy(struct FFrame *self) {
	FAssertPrecondition(self != NULL);
	struct FReference *reference = self->references;
	while(reference != NULL) {
		struct FReference *next = FReferenceGetNextReference(reference);
		FReferenceDestroy(reference);
		reference = next;
	}
	free(self);
}


const char *FFrameGetName(struct FFrame *self) {
	FAssertPrecondition(self != NULL);
	return self->name;
}


struct FFrame *FFrameGetPreviousFrame(struct FFrame *self) {
	FAssertPrecondition(self != NULL);
	return self->prev;
}


struct FReference *FFrameAppendReference(struct FFrame *self, struct FReference *reference) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(reference != NULL);
	if(self->references == NULL) {
		self->references = reference;
	} else {
		FReferenceListAppendReference(self->references, reference);
	}
	return reference;
}

void FFrameVisitReferences(struct FFrame *self, FFrameReferenceVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(visitor != NULL);
	struct FReference *reference = self->references;
	while(reference != NULL) {
		visitor(self, reference, context);
		reference = FReferenceGetNextReference(reference);
	}
}


struct FFrameObjectVisitorState {
	FFrameObjectVisitor visitor;
	void *context;
};

void FFrameVisitObjectForReference(struct FFrame *self, struct FReference *reference, void *context) {
	struct FFrameObjectVisitorState *state = context;
	state->visitor(self, FReferenceGetReferencedObject(reference), state->context);
}

void FFrameVisitObjects(struct FFrame *self, FFrameObjectVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(visitor != NULL);
	struct FFrameObjectVisitorState state = {
		visitor,
		context
	};
	FFrameVisitReferences(self, FFrameVisitObjectForReference, &state);
}


struct FFrame *FFrameListGetFirstFrame(struct FFrame *self) {
	FAssertPrecondition(self != NULL);
	return FFrameGetPreviousFrame(self)?
		FFrameListGetFirstFrame(FFrameGetPreviousFrame(self))
	:	self;
}

struct FFrame *FFrameListAppendFrame(struct FFrame *self, struct FFrame *other) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(other != NULL);
	FAssertPrecondition(self != other);
	
	other->prev = self;
	return other;
}


void FFrameListVisitFrames(struct FFrame *self, FFrameListFrameVisitor visitor, void *context) {
	FAssertPrecondition(self != NULL);
	while(self != NULL) {
		visitor(self, context);
		self = FFrameGetPreviousFrame(self);
	}
}
