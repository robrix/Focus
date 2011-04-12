// FAssertions.h
// Created by Rob Rix on 2011-02-05
// Copyright 2011 Monochrome Industries

#ifndef F_ASSERTIONS

void FAssertionFailure(const char *file, unsigned int line, const char *function, const char *condition, const char *message);

#define FAssertPrecondition(condition) do { if(!(condition)) { FAssertionFailure(__FILE__, __LINE__, __PRETTY_FUNCTION__, #condition, "precondition failed"); } } while(0)

#endif // F_ASSERTIONS
