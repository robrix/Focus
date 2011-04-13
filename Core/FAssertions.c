// FAssertions.m
// Created by Rob Rix on 2011-02-05
// Copyright 2011 Monochrome Industries

#include "FAssertions.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

void FAssertionFailure(const char *file, unsigned int line, const char *function, const char *condition, const char *message) {
	printf("%s:%u: in %s error: %s %s.\n", file, line, function, condition, message);
	fflush(stdout);
	
	void *callstack[128];
	size_t i, frames = backtrace(callstack, 128);
	printf("\nStack trace (%lu frames):\n", frames);
	char **strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
	    printf("%s\n", strs[i]);
	}
	free(strs);
	
	fflush(stdout);
	
	abort();
}
