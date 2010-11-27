// FCompiler.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FMessage.h"
#include "FObject.h"
#include "FCompiler.h"
#include "FFunction.h"
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

LLVMValueRef FCompilerGetReferenceToSelector(FCompiler *compiler, FSymbol *selector) {
	return FCompilerGetReferenceToObject(compiler, (FObject *)selector);
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

LLVMValueRef FCompilerGetSlotFunction(FCompiler *compiler) {
	LLVMTypeRef objectType = FCompilerGetObjectType(compiler);
	return FCompilerGetReferenceToExternalFunction(compiler, "FObjectGetSlot", LLVMFunctionType(objectType, (LLVMTypeRef[]){ objectType, objectType }, 2, 0));
}

LLVMValueRef FCompilerGetFunctionPointerFunction(FCompiler *compiler) {
	LLVMTypeRef objectType = FCompilerGetObjectType(compiler);
	return FCompilerGetReferenceToExternalFunction(compiler, "FFunctionGetFunctionPointer", LLVMFunctionType(LLVMFunctionType(objectType, (LLVMTypeRef[]){ objectType, objectType }, 2, 1), (LLVMTypeRef[]){ objectType }, 1, 0));
}

// fixme: take the function as an argument
LLVMValueRef FCompilerCompileMessage(FCompiler *compiler, FMessage *message) {
	LLVMValueRef receiver = message->receiver
	?	FCompilerCompileMessage(compiler, message->receiver)
	:	FCompilerGetReferenceToObject(compiler, message->context);
	LLVMValueRef selector = FCompilerGetReferenceToSelector(compiler, message->selector);
	FMessageNode *node = message->arguments;
	size_t count = FMessageNodeGetNodeCount(message->arguments), i = 2;
	LLVMValueRef arguments[count + 2];
	arguments[0] = receiver;
	arguments[1] = selector;
	do {
		arguments[i++] = FCompilerCompileMessage(compiler, node->message);
	} while((node = node->nextNode));
	LLVMValueRef function = LLVMBuildCall(compiler->builder, FCompilerGetSlotFunction(compiler), (LLVMValueRef[]){ receiver, selector }, 2, "lookup");
	LLVMValueRef functionPointer = LLVMBuildCall(compiler->builder, FCompilerGetFunctionPointerFunction(compiler), (LLVMValueRef[]){ function }, 1, "get fptr");
	return LLVMBuildCall(compiler->builder, LLVMBuildPointerCast(compiler->builder, functionPointer, FCompilerGetMethodTypeOfArity(compiler, FSymbolGetArity(message->selector)), "cast fptr to method type"), arguments, count, FSymbolGetString(message->selector));
}


LLVMValueRef FCompilerCompileFunction(FCompiler *compiler, FFunction *function) {
	LLVMValueRef result = NULL;
	while((node = FNodeGetNextObject(node))) {
		result = FCompilerCompileMessage(compiler, FNodeGetObject(message));
	}
	// compile each message, return the last one
	// pass the function to each message so they can check for references to the arguments
	
	return result;
}
