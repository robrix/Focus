// FFrame.h
// Created by Rob Rix on 2011-04-11
// Copyright 2011 Monochrome Industries

#ifndef F_FRAME
#define F_FRAME

struct FFrame *FFrameCreate(const char *name);
void FFrameDestroy(struct FFrame *self);

const char *FFrameGetName(struct FFrame *self);

struct FFrame *FFrameGetPreviousFrame(struct FFrame *self);

struct FReference *FFrameAppendReference(struct FFrame *self, struct FReference *reference);

typedef void (*FFrameReferenceVisitor)(struct FFrame *frame, struct FReference *reference, void *context);
void FFrameVisitReferences(struct FFrame *self, FFrameReferenceVisitor visitor, void *context);

typedef void (*FFrameObjectVisitor)(struct FFrame *frame, struct FObject *object, void *context);
void FFrameVisitObjects(struct FFrame *self, FFrameObjectVisitor visitor, void *context);

struct FFrame *FFrameListGetFirstFrame(struct FFrame *self);
struct FFrame *FFrameListAppendFrame(struct FFrame *self, struct FFrame *other);

typedef void (*FFrameListFrameVisitor)(struct FFrame *frame, void *context);
void FFrameListVisitFrames(struct FFrame *self, FFrameListFrameVisitor visitor, void *context);

#endif // F_FRAME
