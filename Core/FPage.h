// FPage.h
// Created by Rob Rix on 2011-04-03
// Copyright 2011 Monochrome Industries

#ifndef F_PAGE
#define F_PAGE

#include <stdlib.h>

#include "FObject.h"

struct FPage *FPageCreate();
void FPageDestroy(struct FPage *page);

FObject *FPageAllocateObject(struct FPage *page);

typedef void (*FPageObjectVisitor)(struct FPage *page, FObject *object, void *context);
void FPageVisitObjects(struct FPage *page, FPageObjectVisitor visitor, void *context);

#endif // F_PAGE
