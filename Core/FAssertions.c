// FAssertions.m
// Created by Rob Rix on 2011-02-05
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"

#include <stdio.h>
#include <stdlib.h>

void FAssertionFailure(const char *file, unsigned int line, const char *function, const char *condition, const char *message) {
	printf("%s:%u: in %s error: %s %s.\n", file, line, function, condition, message);
	fflush(stdout);
	abort();
}
