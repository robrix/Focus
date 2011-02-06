// FAssertions.m
// Created by Rob Rix on 2011-02-05
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"

#include <stdio.h>
#include <stdlib.h>

void FAssertionFailure(const char *file, unsigned int line, const char *condition, const char *message) {
	printf("%s:%u: error: %s %s.\n", file, line, condition, message);
	fflush(stdout);
	abort();
}
