// FCompiler.h
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#ifndef F_COMPILER
#define F_COMPILER

#define __STDC_CONSTANT_MACROS
#define __STDC_LIMIT_MACROS

#include <llvm-c/Core.h>
#include "FObject.h"

FObject *FCompilerCreate();

FImplementation FCompilerCompileFunction(FObject *compiler, FObject *function);

#endif // F_COMPILER
