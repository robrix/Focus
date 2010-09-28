// FSymbol.c
// Created by Rob Rix on 2010-09-11
// Copyright 2010 Monochrome Industries

#include "FSymbol.h"
#include <stdlib.h>
#include <string.h>

FSymbol *FSymbolCreate() {
	return NULL;
}


bool FSymbolIsEqual(FSymbol *a, FSymbol *b) {
	return
		((a == b) && (a != NULL))
	||	(
		(a->hash == b->hash)
	&&	(strcmp(a->symbol, b->symbol) == 0)
	);
}
