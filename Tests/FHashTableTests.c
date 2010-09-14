// FHashTableTests.c
// Created by Rob Rix on 2010-09-14
// Copyright 2010 Monochrome Industries

#include "FHashTable.h"
#include <stdio.h>

__attribute__((constructor))
static void FTestHashTable() {
	printf("FTestHashTable()\n");
}
