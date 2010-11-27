// FListNode.c
// Created by Rob Rix on 2010-11-19
// Copyright 2010 Monochrome Industries

#include "FListNode.h"
#include "FSymbol.h"
#include "FObject+Protected.h"
#include "Prototypes/FListNodePrototype.h"

FListNode *FListNodeCreate(FListNode *object) {
	FListNode *node = FObjectCreate(FListNodePrototypeGet());
	FObjectSetVariable(node, FSymbolCreateWithString("object"), object);
	return node;
}


void FListNodeSetNextNode(FListNode *self, FListNode *nextNode) {
	FObjectSetVariable(self, FSymbolCreateWithString("nextNode"), nextNode);
}

FListNode *FListNodeGetNextNode(FListNode *self) {
	return FObjectGetVariable(self, FSymbolCreateWithString("nextNode"));
}

FListNode *FListNodeGetLastNode(FListNode *self) {
	FListNode *node = self;
	while(FListNodeGetNextNode(node) != NULL) {
		node = FListNodeGetNextNode(node);
	}
	return node;
}


size_t FListNodeGetNodeCount(FListNode *self) {
	size_t count = 0;
	do {
		count++;
	} while((self = FListNodeGetNextNode(self)));
	return count;
}


struct FObject *FListNodeGetObject(FListNode *self) {
	return FObjectGetVariable(self, FSymbolCreateWithString("object"));
}
