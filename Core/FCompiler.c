// FCompiler.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FObject.h"
#include "FCompiler.h"
#include "FFunction.h"
#include "Prototypes/FMessagePrototype.h"
#include "Prototypes/FListNodePrototype.h"
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <stdlib.h>
#include <stdio.h>

struct FCompiler {
	LLVMContextRef context;
	LLVMModuleRef module;
	LLVMBuilderRef builder;
	LLVMExecutionEngineRef executor;
};

FCompiler *FCompilerCreate() {
	static bool initedJIT = 0;
	if(!initedJIT) {
		LLVMLinkInJIT();
		LLVMInitializeNativeTarget();
		initedJIT = 1;
	}
	FCompiler *compiler = FAllocatorAllocate(NULL, sizeof(FCompiler));
	compiler->context = LLVMContextCreate();
	compiler->module = LLVMModuleCreateWithNameInContext("Focus", compiler->context);
	compiler->builder = LLVMCreateBuilderInContext(compiler->context);
	LLVMCreateJITCompilerForModule(&compiler->executor, compiler->module, 3, NULL);
	return compiler;
}

LLVMTypeRef FCompilerGetObjectType(FCompiler *compiler) {
	LLVMTypeRef type = LLVMGetTypeByName(compiler->module, "FObject");
	if(!type) {
		type = LLVMPointerType(LLVMInt8TypeInContext(compiler->context), 0);
		LLVMAddTypeName(compiler->module, "FObject", type);
	}
	return type;
}


LLVMValueRef FCompilerGetReferenceToObject(FCompiler *compiler, FObject *object) {
	return LLVMConstIntToPtr(LLVMConstInt(LLVMInt8TypeInContext(compiler->context), (unsigned long long)object, 0), FCompilerGetObjectType(compiler));
}

LLVMValueRef FCompilerGetReferenceToExternalFunction(FCompiler *compiler, const char *name, LLVMTypeRef type) {
	LLVMValueRef function = LLVMGetNamedFunction(compiler->module, name);
	if(!function) {
		function = LLVMAddFunction(compiler->module, name, type);
		LLVMSetLinkage(function, LLVMExternalLinkage);
	}
	return function;
}

LLVMTypeRef FCompilerGetMethodTypeOfArity(FCompiler *compiler, size_t arity) {
	// there are two implicit parameters, self and selector.
	LLVMTypeRef types[arity + 2];
	for(size_t i = 0; i < arity + 2; i++) {
		types[i] = FCompilerGetObjectType(compiler);
	}
	return LLVMFunctionType(FCompilerGetObjectType(compiler), types, arity + 2, 0);
}

LLVMValueRef FCompilerGetMethodFunction(FCompiler *compiler) {
	LLVMTypeRef objectType = FCompilerGetObjectType(compiler);
	return FCompilerGetReferenceToExternalFunction(compiler, "FObjectGetMethod", LLVMFunctionType(objectType, (LLVMTypeRef[]){ objectType, objectType }, 2, 0));
}

LLVMValueRef FCompilerGetImplementationFunction(FCompiler *compiler) {
	LLVMTypeRef objectType = FCompilerGetObjectType(compiler);
	return FCompilerGetReferenceToExternalFunction(compiler, "FFunctionGetImplementation", LLVMFunctionType(LLVMFunctionType(objectType, (LLVMTypeRef[]){ objectType, objectType }, 2, 1), (LLVMTypeRef[]){ objectType }, 1, 0));
}

// fixme: if the message is nullary and has no receiver, try it out as an argument reference
LLVMValueRef FCompilerCompileMessage(FCompiler *compiler, FObject *function, FObject *message) {
	if(!message) {
		printf("attempting to compile null message!\n");
		fflush(stdout);
	}
	LLVMValueRef receiver = FSend(message, receiver)
	?	FCompilerCompileMessage(compiler, function, FSend(message, receiver))
	:	FCompilerGetReferenceToObject(compiler, FSend(message, context));
	LLVMValueRef selector = FCompilerGetReferenceToObject(compiler, FSend(message, selector));
	FObject *argumentNode = FSend(message, arguments);
	size_t count = FListNodeGetCount(argumentNode), i = 2;
	LLVMValueRef arguments[count + 2];
	arguments[0] = receiver;
	arguments[1] = selector;
	while(argumentNode) {
		arguments[i++] = FCompilerCompileMessage(compiler, function, FSend(argumentNode, object));
		argumentNode = FSend(argumentNode, next);
	}
	LLVMValueRef method = LLVMBuildCall(compiler->builder, FCompilerGetMethodFunction(compiler), arguments, 2, "");
	LLVMValueRef implementation = LLVMBuildCall(compiler->builder, FCompilerGetImplementationFunction(compiler), (LLVMValueRef[]){ method }, 1, "get fptr");
	return LLVMBuildCall(compiler->builder, LLVMBuildPointerCast(compiler->builder, implementation, FCompilerGetMethodTypeOfArity(compiler, FSymbolGetArity(FSend(message, selector))), "cast fptr to method type"), arguments, count + 2, FSymbolGetString(FSend(message, selector)));
}


FImplementation FCompilerCompileFunction(FCompiler *compiler, FObject *function) {
	// pick a name for the function; "function 0x…" or similar will do for now
	LLVMValueRef f = LLVMAddFunction(compiler->module, "function", FCompilerGetMethodTypeOfArity(compiler, FFunctionGetArity(function))), result = NULL;
	LLVMAppendBasicBlock(f, "entry");
	LLVMPositionBuilderAtEnd(compiler->builder, LLVMGetEntryBasicBlock(f));
	
	FObject *messageNode = FSend(function, messages);
	while(messageNode) {
		result = FCompilerCompileMessage(compiler, function, FSend(messageNode, object));
		messageNode = FSend(messageNode, next);
	}
	
	LLVMBuildRet(compiler->builder, result);
	
	LLVMVerifyFunction(f, LLVMAbortProcessAction);
	
	return LLVMRecompileAndRelinkFunction(compiler->executor, f);
}
