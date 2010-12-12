// FCompiler.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FObject+Protected.h"
#include "FCompiler.h"
#include "Prototypes/FFunctionPrototype.h"
#include "Prototypes/FMessagePrototype.h"
#include "Prototypes/FFunctionPrototype.h"
#include "Prototypes/FListNodePrototype.h"
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <stdlib.h>
#include <stdio.h>

LLVMContextRef FCompilerGetContext(FObject *self) {
	return (LLVMContextRef)FObjectGetVariable(self, FSymbolCreateWithString(" context"));
}

LLVMModuleRef FCompilerGetModule(FObject *self) {
	return (LLVMModuleRef)FObjectGetVariable(self, FSymbolCreateWithString(" module"));
	
}

LLVMBuilderRef FCompilerGetBuilder(FObject *self) {
	return (LLVMBuilderRef)FObjectGetVariable(self, FSymbolCreateWithString(" builder"));
	
}

LLVMExecutionEngineRef FCompilerGetExecutor(FObject *self) {
	return (LLVMExecutionEngineRef)FObjectGetVariable(self, FSymbolCreateWithString(" executor"));
}


FObject *FCompilerCreate() {
	static bool initedJIT = 0;
	if(!initedJIT) {
		LLVMLinkInJIT();
		LLVMInitializeNativeTarget();
		initedJIT = 1;
	}
	FObject *compiler = FObjectCreate(NULL);
	LLVMContextRef context = LLVMContextCreate();
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("Focus", context);
	LLVMExecutionEngineRef executor = NULL;
	LLVMCreateJITCompilerForModule(&executor, module, 3, NULL);
	FObjectSetVariable(compiler, FSymbolCreateWithString(" context"), (FObject *)context);
	FObjectSetVariable(compiler, FSymbolCreateWithString(" module"), (FObject *)module);
	FObjectSetVariable(compiler, FSymbolCreateWithString(" builder"), (FObject *)LLVMCreateBuilderInContext(context));
	FObjectSetVariable(compiler, FSymbolCreateWithString(" executor"), (FObject *)executor);
	
	return compiler;
}

LLVMTypeRef FCompilerGetObjectType(FObject *compiler) {
	LLVMTypeRef type = LLVMGetTypeByName(FCompilerGetModule(compiler), "FObject");
	if(!type) {
		type = LLVMPointerType(LLVMInt8TypeInContext(FCompilerGetContext(compiler)), 0);
		LLVMAddTypeName(FCompilerGetModule(compiler), "FObject", type);
	}
	return type;
}


LLVMValueRef FCompilerGetReferenceToObject(FObject *compiler, FObject *object) {
	return LLVMConstIntToPtr(LLVMConstInt(LLVMInt8TypeInContext(FCompilerGetContext(compiler)), (unsigned long long)object, 0), FCompilerGetObjectType(compiler));
}

LLVMValueRef FCompilerGetReferenceToExternalFunction(FObject *compiler, const char *name, LLVMTypeRef type) {
	LLVMValueRef function = LLVMGetNamedFunction(FCompilerGetModule(compiler), name);
	if(!function) {
		function = LLVMAddFunction(FCompilerGetModule(compiler), name, type);
		LLVMSetLinkage(function, LLVMExternalLinkage);
	}
	return function;
}

LLVMTypeRef FCompilerGetMethodTypeOfArity(FObject *compiler, size_t arity) {
	// there are two implicit parameters, self and selector.
	LLVMTypeRef types[arity + 2];
	for(size_t i = 0; i < arity + 2; i++) {
		types[i] = FCompilerGetObjectType(compiler);
	}
	return LLVMFunctionType(FCompilerGetObjectType(compiler), types, arity + 2, 0);
}

LLVMValueRef FCompilerGetMethodFunction(FObject *compiler) {
	LLVMTypeRef objectType = FCompilerGetObjectType(compiler);
	return FCompilerGetReferenceToExternalFunction(compiler, "FObjectGetMethod", LLVMFunctionType(objectType, (LLVMTypeRef[]){ objectType, objectType }, 2, 0));
}

LLVMValueRef FCompilerGetImplementationFunction(FObject *compiler) {
	LLVMTypeRef objectType = FCompilerGetObjectType(compiler);
	return FCompilerGetReferenceToExternalFunction(compiler, "FFunctionGetImplementation", LLVMFunctionType(LLVMPointerType(LLVMFunctionType(objectType, (LLVMTypeRef[]){ objectType, objectType }, 2, 1), 0), (LLVMTypeRef[]){ objectType }, 1, 0));
}


LLVMValueRef FCompilerCompileMessage(FObject *compiler, FObject *function, FObject *context, FObject *message) {
	if(!message) {
		printf("attempting to compile null message!\n");
		fflush(stdout);
	}
	
	LLVMValueRef result = NULL;
	LLVMValueRef selector = FCompilerGetReferenceToObject(compiler, FSend(message, selector));
	size_t paramIndex = FFunctionGetIndexOfArgument(function, FSend(message, selector));
	if(paramIndex != FFunctionArgumentNotFound) {
		result = LLVMGetParam(LLVMGetBasicBlockParent(LLVMGetInsertBlock(FCompilerGetBuilder(compiler))), paramIndex + 2);
	} else {
		LLVMValueRef receiver = FSend(message, receiver)
		?	FCompilerCompileMessage(compiler, function, context, FSend(message, receiver))
		:	FCompilerGetReferenceToObject(compiler, context);
		FObject *argumentNode = FSend(message, arguments);
		size_t count = FListNodeGetCount(argumentNode), i = 2;
		LLVMValueRef arguments[count + 2];
		arguments[0] = receiver;
		arguments[1] = selector;
		while(argumentNode) {
			arguments[i++] = FCompilerCompileMessage(compiler, function, context, FSend(argumentNode, object));
			argumentNode = FSend(argumentNode, next);
		}
		LLVMValueRef method = LLVMBuildCall(FCompilerGetBuilder(compiler), FCompilerGetMethodFunction(compiler), arguments, 2, "");
		LLVMValueRef implementation = LLVMBuildCall(FCompilerGetBuilder(compiler), FCompilerGetImplementationFunction(compiler), (LLVMValueRef[]){ method }, 1, "get fptr");
		result = LLVMBuildCall(FCompilerGetBuilder(compiler), LLVMBuildPointerCast(FCompilerGetBuilder(compiler), implementation, LLVMPointerType(FCompilerGetMethodTypeOfArity(compiler, FSymbolGetArity(FSend(message, selector))), 0), "cast fptr to method type"), arguments, count + 2, FSymbolGetString(FSend(message, selector)));
	}
	return result;
}


FImplementation FCompilerCompileFunction(FObject *compiler, FObject *function) {
	if(!function) {
		printf("attempting to compile null function!\n");
		fflush(stdout);
		return NULL;
	}
	
	// pick a name for the function; "function 0x…" or similar will do for now
	LLVMValueRef f = LLVMAddFunction(FCompilerGetModule(compiler), "function", FCompilerGetMethodTypeOfArity(compiler, FFunctionGetArity(function))), result = NULL;
	LLVMAppendBasicBlock(f, "entry");
	LLVMPositionBuilderAtEnd(FCompilerGetBuilder(compiler), LLVMGetEntryBasicBlock(f));
	
	// set up a new context inheriting from the function’s context
	FObject *context = FObjectCreate(FSend(function, context));
	
	FObject *messageNode = FSend(function, messages);
	while(messageNode) {
		result = FCompilerCompileMessage(compiler, function, context, FSend(messageNode, object));
		messageNode = FSend(messageNode, next);
	}
	
	LLVMBuildRet(FCompilerGetBuilder(compiler), result);
	
	LLVMVerifyFunction(f, LLVMAbortProcessAction);
	// optimize the function
	
	return LLVMRecompileAndRelinkFunction(FCompilerGetExecutor(compiler), f);
}
