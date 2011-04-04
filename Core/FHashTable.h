// FHashTable.h
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#ifndef F_HASH_TABLE
#define F_HASH_TABLE

#include "FObject.h"

typedef struct FSlot {
	FSymbol symbol;
	void *value;
	struct FSlot *next;
} FSlot;

typedef struct FHashTable {
	uint16_t slotCount; uint16_t bucketCount;
	FSlot slots[]; // variable-length structure
} FHashTable;

typedef void (*FHashTableSlotVisitor)(FHashTable *table, FSlot *slot, void *context);

FHashTable *FHashTableCreate();
FHashTable FHashTableMake();

size_t FHashTableGetSize(FHashTable *self);
size_t FHashTableGetSizeForSlotCount(uint16_t slotCount);

void *FHashTableGetValueForKey(FHashTable *self, FSymbol *symbol);
void FHashTableSetValueForKey(FHashTable *self, FSymbol *symbol, void *value); // fixme: assumes you aren’t inserting a duplicate entry for the given key

void FHashTableVisitSlots(FHashTable *self, FHashTableSlotVisitor visitor, void *context);

#endif // F_HASH_TABLE
