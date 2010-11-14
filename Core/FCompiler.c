// FCompiler.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FMessage.h"
#include "FObject.h"
#include "FCompiler.h"
#include <llvm-c/ExecutionEngine.h>
#include <stdlib.h>

struct FCompiler {
	LLVMContextRef context;
	LLVMModuleRef module;
	LLVMBuilderRef builder;
	LLVMExecutionEngineRef executor;
};

FCompiler *FCompilerCreate() {
	FCompiler *compiler = FAllocatorAllocate(NULL, sizeof(FCompiler));
	
	return compiler;
}


LLVMValueRef FCompilerGetReferenceToContext(FCompiler *compiler, FObject *context) {
	return NULL;
}

LLVMValueRef FCompilerGetReferenceToSelector(FCompiler *compiler, FSymbol *selector) {
	return NULL;
}

LLVMValueRef FCompilerGetMethodFunction(FCompiler *compiler) {
	return NULL;
}

LLVMValueRef FCompilerCompileMessage(FCompiler *compiler, FMessage *message) {
	LLVMValueRef receiver = message->receiver
	?	FCompilerCompileMessage(compiler, message->receiver)
	:	FCompilerGetReferenceToContext(compiler, message->context);
	LLVMValueRef selector = FCompilerGetReferenceToSelector(compiler, message->selector);
	FMessageNode *node = message->arguments;
	size_t count = FMessageNodeGetNodeCount(message->arguments), i = 2;
	LLVMValueRef arguments[count + 2];
	arguments[0] = receiver;
	arguments[1] = selector;
	do {
		arguments[i++] = FCompilerCompileMessage(compiler, node->message);
	} while((node = node->nextNode));
	LLVMValueRef method = LLVMBuildCall(compiler->builder, FCompilerGetMethodFunction(compiler), (LLVMValueRef[]){receiver, selector}, 2, "lookup");
	return LLVMBuildCall(compiler->builder, method, arguments, count, FSymbolGetString(message->selector));
}
