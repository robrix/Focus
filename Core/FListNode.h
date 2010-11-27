// FListNode.h
// Created by Rob Rix on 2010-11-19
// Copyright 2010 Monochrome Industries

#ifndef F_LIST_NODE
#define F_LIST_NODE

#include <stdlib.h>

typedef struct FObject FListNode;

FListNode *FListNodeCreate(FListNode *object);
void FListNodeSetNextNode(FListNode *self, FListNode *nextNode);
FListNode *FListNodeGetNextNode(FListNode *self);
FListNode *FListNodeGetLastNode(FListNode *self);
size_t FListNodeGetNodeCount(FListNode *self);
struct FObject *FListNodeGetObject(FListNode *self);

#endif // F_LIST_NODE
