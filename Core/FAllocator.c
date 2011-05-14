// FAllocator.c
// Created by Rob Rix on 2010-09-28
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FAssertions.h"
#include "FFrame.h"
#include "FPage.h"
#include "FReference.h"
#include "FObject+Protected.h"
#include <stdint.h>

// should there be one allocator per thread? (at the very least, a shared allocator isn’t thread safe)
// there isn’t a fixed # of generations, rather, generations are allocated as needed when collections are done
// objects larger than the generation size need to be handled separately
typedef struct FAllocator {
	struct FPage *generations;
	struct FFrame *callStack;
} FAllocator;


FAllocator *FAllocatorCreate() {
	FAllocator *allocator = calloc(1, sizeof(FAllocator));
	allocator->generations = FPageCreate(allocator);
	return allocator;
}

void FAllocatorDestroy(FAllocator *self) {
	FAssertPrecondition(self != NULL);
	struct FPage *generation = self->generations;
	while(generation != NULL) {
		FPageDestroy(generation);
		generation = FPageGetNextPage(generation);
	}
	free(self);
}


struct FPage *FAllocatorGetNursery(FAllocator *self) {
	FAssertPrecondition(self != NULL);
	return self->generations;
}


struct FObject *FAllocatorAllocateObjectWithSlotCount(struct FAllocator *self, uint16_t slotCount) {
	FAssertPrecondition(self != NULL);
	FObject *object = FPageAllocateObjectWithSlotCount(FAllocatorGetNursery(self), slotCount);
	if(!object) {
		FAllocatorCollect(self);
		// perform a collection, promoting live objects to the next generation
		// then allocate again in the nursery
	}
	return object;
}

FObject *FAllocatorAllocateObject(FAllocator *self) {
	FAssertPrecondition(self != NULL);
	return FAllocatorAllocateObjectWithSlotCount(self, 0);
	// FObject *object = FPageAllocateObject(FAllocatorGetNursery(self));
	// if(!object) {
	// 	FAllocatorCollect(self);
	// 	// perform a collection, promoting live objects to the next generation
	// 	// then allocate again in the nursery
	// }
	// return object;
}

void *FAllocatorAllocate(FAllocator *self, size_t bytes) {
	FAssertPrecondition(self != NULL);
	#pragma message("fixme: handle bytes > F_ALLOCATOR_GENERATION_SIZE specially")
	return calloc(1, bytes);
}

void *FAllocatorResizeAllocation(struct FAllocator *self, void *allocation, size_t bytes) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(allocation != NULL);
	
	return realloc(allocation, bytes);
}


struct FPage *FAllocatorGetPageForObject(struct FAllocator *self, struct FObject *object) {
	struct FPage *page = self->generations;
	while((page != NULL) && !FPageContainsAddress(page, object)) {
		page = FPageGetNextPage(page);
	}
	return page;
}


struct FAllocatorCopyReferencesState {
	struct FObject *original, *copy;
	struct FPage *page;
	struct FReference *references;
};

void FAllocatorCopyReferenceInFrame(struct FFrame *frame, struct FReference *reference, void *context) {
	struct FAllocatorCopyReferencesState *state = context;
	if(FReferenceGetReferencedObject(reference) == state->original) {
		struct FReference *copy = FReferenceCreateCopy(reference);
		if(state->references != NULL) {
			FReferenceListAppendReference(state->references, copy);
		} else {
			state->references = copy;
		}
	}
}

void FAllocatorCopyReferencesInFrame(struct FFrame *frame, void *context) {
	FFrameVisitReferences(frame, FAllocatorCopyReferenceInFrame, context);
}

void FAllocatorCopyReferenceInPage(struct FPage *page, struct FObject *object, struct FReference *reference, void *context) {
	struct FAllocatorCopyReferencesState *state = context;
	if(FReferenceGetReferencedObject(reference) == state->original) {
		struct FReference *copy = FReferenceCreateCopy(reference);
		if(state->references != NULL) {
			FReferenceListAppendReference(state->references, copy);
		} else {
			state->references = copy;
		}
	}
}

void FAllocatorCopyLiveReferenceInPage(struct FPage *page, struct FObject *object, struct FReference *reference, void *context) {
	struct FAllocatorCopyReferencesState *state = context;
	if((object != state->original) && FAllocatorObjectIsLive(FPageGetAllocator(page), object)) {
		FAllocatorCopyReferenceInPage(page, object, reference, context);
	}
}

void FAllocatorCopyReferencesInPage(struct FPage *page, void *context) {
	struct FAllocatorCopyReferencesState *state = context;
	if(page != state->page) { // ignore the page we’re collecting, so we don’t cause retain cycles
		FPageVisitReferences(page, FAllocatorCopyReferenceInPage, context);
	}
}

void FAllocatorUpdateReferenceToObject(struct FReference *reference, void *context) {
	FReferenceSetReferencedObject(reference, (struct FObject *)context);
}


void FAllocatorCopyExternalHeapReferencesToObject(struct FAllocator *self, struct FObject *object, struct FAllocatorCopyReferencesState *state) {
	FPageListVisitPages(FAllocatorGetNursery(self), FAllocatorCopyReferencesInPage, state);
}

void FAllocatorCopyInternalLiveHeapReferencesToObject(struct FAllocator *self, struct FObject *object, struct FAllocatorCopyReferencesState *state) {
	FPageVisitReferences(state->page, FAllocatorCopyLiveReferenceInPage, state);
}

void FAllocatorCopyFrameReferencesToObject(struct FAllocator *self, struct FObject *object, struct FAllocatorCopyReferencesState *state) {
	FFrameListVisitFrames(FAllocatorGetCurrentFrame(self), FAllocatorCopyReferencesInFrame, state);
}

bool FAllocatorObjectIsLive(struct FAllocator *self, struct FObject *object) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(object != NULL);
	struct FAllocatorCopyReferencesState state = {
		.original = object,
		.page = FAllocatorGetPageForObject(self, object)
	};
	FAllocatorCopyExternalHeapReferencesToObject(self, object, &state);
	FAllocatorCopyInternalLiveHeapReferencesToObject(self, object, &state);
	FAllocatorCopyFrameReferencesToObject(self, object, &state);
	return state.references != NULL;
}

struct FReference *FAllocatorCopyReferencesToObject(struct FAllocator *self, struct FPage *page, struct FObject *object) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(page != NULL);
	FAssertPrecondition(object != NULL);
	struct FAllocatorCopyReferencesState state = {
		.original = object,
		.page = page
	};
	FAllocatorCopyExternalHeapReferencesToObject(self, object, &state);
	FAllocatorCopyInternalLiveHeapReferencesToObject(self, object, &state);
	FAllocatorCopyFrameReferencesToObject(self, object, &state);
	return state.references;
}

void FAllocatorCollectObjectInPage(struct FPage *page, struct FObject *object, void *context) {
	FAllocator *self = context;
	
	struct FReference *references = FAllocatorCopyReferencesToObject(self, page, object);
	if(references != NULL) {
		struct FPage *next = FPageGetNextPage(page) ?: FPageListAppendPage(page, FPageCreate(self));
		
		struct FObject *copy = FPageCopyObject(next, object);
		FReferenceListVisitReferences(references, FAllocatorUpdateReferenceToObject, copy);
	}
}

void FAllocatorCollectPage(struct FAllocator *self, struct FPage *page) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(page != NULL);
	
	FPageVisitObjects(page, FAllocatorCollectObjectInPage, self);
	
	FPageDrain(page);
}

void FAllocatorCollect(struct FAllocator *self) {
	FAssertPrecondition(self != NULL);
	FAllocatorCollectPage(self, FAllocatorGetNursery(self));
}


struct FFrame *FAllocatorGetCurrentFrame(struct FAllocator *self) {
	FAssertPrecondition(self != NULL);
	return self->callStack;
}

struct FFrame *FAllocatorPushFrame(struct FAllocator *self, const char *function)
{
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(function != NULL);
	struct FFrame *frame = FFrameCreate(function);
	return self->callStack = (FAllocatorGetCurrentFrame(self) == NULL)?
		frame
	:	FFrameListAppendFrame(self->callStack, frame);
}

struct FObject *FAllocatorMakeStrongReferenceToObjectAtAddress(struct FAllocator *self, void **address, size_t offset) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(address != NULL);
	FFrameAppendReference(FAllocatorGetCurrentFrame(self), FReferenceCreate((void **)address, offset));
	return *address;
}

void FAllocatorPopFrame(struct FAllocator *self)
{
	FAssertPrecondition(self != NULL);
	struct FFrame *currentFrame = FAllocatorGetCurrentFrame(self);
	FAssertPrecondition(currentFrame != NULL);
	self->callStack = FFrameGetPreviousFrame(currentFrame);
	FFrameDestroy(currentFrame);
}
