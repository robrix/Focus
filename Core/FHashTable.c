// FHashTable.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FAssertions.h"
#include "FSymbol.h"
#include "FHashTable.h"
#include <stdlib.h>
#include <stdio.h>

FHashTable *FHashTableCreate() {
	FHashTable *instance = FAllocatorAllocate(NULL, FHashTableGetSizeForSlotCount(0 + 17));
	*instance = FHashTableMake();
	return instance;
}

FHashTable FHashTableMake() {
	return (FHashTable){
		.slotCount = 0, .bucketCount = 0
	};
}



size_t FHashTableGetSize(FHashTable *self) {
	FAssertPrecondition(self != NULL);
	return FHashTableGetSizeForSlotCount(self->slotCount);
}

size_t FHashTableGetSizeForSlotCount(uint16_t slotCount) {
	return
		sizeof(uint32_t) + sizeof(uint32_t) // slot and bucket counts
	+	(sizeof(FSlot) * slotCount)
	;
}


FSlot *FHashTableGetBucketForHash(FHashTable *self, FUInteger hash) {
	FAssertPrecondition(self != NULL);
	return (self->bucketCount > 0)? (self->slots + (hash % self->bucketCount)) : NULL;
}

FSlot *FHashTableGetSlotForKey(FHashTable *self, FSymbol *symbol) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(symbol != NULL);
	FAssertPrecondition(symbol->key != NULL);
	FSlot *slot = FHashTableGetBucketForHash(self, symbol->hash);
	while((slot != NULL) && (slot->symbol.key != NULL)) {
		if(FSymbolIsEqual(&(slot->symbol), symbol)) {
			break;
		} else {
			slot = slot->next;
		}
	}
	return slot;
}

void *FHashTableGetValueForKey(FHashTable *self, FSymbol *symbol) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(symbol != NULL);
	FSlot *slot = FHashTableGetSlotForKey(self, symbol);
	return
		slot
	?	slot->value
	:	NULL;
}


FSlot *FSlotGetTail(FSlot *slot) {
	FAssertPrecondition(slot != NULL);
	while(slot->next != NULL) {
		slot = slot->next;
	}
	return slot;
}

// may cause the table to move in memory, so everything needs to account for that
FSlot *FHashTableAddSlots(FHashTable *self, uint8_t n) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(n > 0);
	size_t originalSize = FHashTableGetSize(self);
	
	// resize the object
	self = FAllocatorResizeAllocation(NULL, self, FHashTableGetSizeForSlotCount(self->slotCount += n));
	
	// NULL out the new slot
	FSlot *slot = (FSlot *)(self + originalSize);
	for(uint32_t i = 0; i < n; i++, slot++) {
		*slot = (FSlot){{0}, NULL, NULL};
	}
	return slot;
}

void FHashTableSetValueForKey(FHashTable *self, FSymbol *symbol, void *value) {
	FAssertPrecondition(self != NULL);
	FAssertPrecondition(symbol != NULL);
	FAssertPrecondition(symbol->key != NULL);
	
	if(self->bucketCount == 0) {
		FHashTableAddSlots(self, self->bucketCount = 3);
	}
	
	// if the slot exists, use it; otherwise make a new one
	FSlot *slot = FHashTableGetSlotForKey(self, symbol);
	if(slot == NULL) {
		slot = FHashTableGetBucketForHash(self, symbol->hash);
		if(slot->symbol.key != NULL) {
			FSlot *last = FSlotGetTail(slot);
			slot = FHashTableAddSlots(self, 1);
			last->next = slot;
		}
	}
	
	slot->symbol = *symbol;
	slot->value = value;
}
