// FPage.h
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#ifndef F_PAGE
#define F_PAGE

#include "FObject.h"

struct FPage *FPageCreate();
void FPageDestroy(struct FPage *self);

struct FPage *FPageGetNextPage(struct FPage *self);

struct FObject *FPageAllocateObject(struct FPage *self);
struct FObject *FPageCopyObject(struct FPage *self, struct FObject *original);

void FPageDrain(struct FPage *self);

typedef void (*FPageObjectVisitor)(struct FPage *self, struct FObject *object, void *context);
void FPageVisitObjects(struct FPage *self, FPageObjectVisitor visitor, void *context);

struct FPage *FPageListGetLastPage(struct FPage *self);
struct FPage *FPageListAppendPage(struct FPage *self, struct FPage *other);

typedef void (*FPageListPageVisitor)(struct FPage *page, void *context);
void FPageListVisitPages(struct FPage *self, FPageListPageVisitor visitor, void *context);

#endif // F_PAGE
