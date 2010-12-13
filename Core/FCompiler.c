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
	return (LLVMContextRef)FSend(self, $context);
}

LLVMModuleRef FCompilerGetModule(FObject *self) {
	return (LLVMModuleRef)FSend(self, $module);
	
}

LLVMBuilderRef FCompilerGetBuilder(FObject *self) {
	return (LLVMBuilderRef)FSend(self, $builder);
	
}

LLVMExecutionEngineRef FCompilerGetExecutor(FObject *self) {
	return (LLVMExecutionEngineRef)FSend(self, $executor);
}


LLVMValueRef FCompilerVisitMessageWithVisitedReceiverAndArguments(FObject *self, FObject *selector, FObject *message, LLVMValueRef receiver, LLVMValueRef arguments[]);
FObject *FCompilerVisitFunction(FObject *self, FObject *selector, FObject *function);

FObject *FCompilerCreate() {
	static bool initedJIT = 0;
	if(!initedJIT) {
		LLVMLinkInJIT();
		LLVMInitializeNativeTarget();
		initedJIT = 1;
	}
	FObject *compiler = FObjectCreate(NULL);
	
	FObjectSetMethod(compiler, FSymbolCreateWithString("visitMessage:withVisitedReceiver:visitedArguments:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FCompilerVisitMessageWithVisitedReceiverAndArguments));
	FObjectSetMethod(compiler, FSymbolCreateWithString("visitFunction:"), FFunctionCreateWithImplementation(NULL, (FImplementation)FCompilerVisitFunction));
	
	
	LLVMContextRef context = LLVMContextCreate();
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("Focus", context);
	LLVMExecutionEngineRef executor = NULL;
	LLVMCreateJITCompilerForModule(&executor, module, 3, NULL);
	FObjectSetMethod(compiler, FSymbolCreateWithString("$context"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(compiler, FSymbolCreateWithString("$module"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(compiler, FSymbolCreateWithString("$builder"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(compiler, FSymbolCreateWithString("$executor"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetVariable(compiler, FSymbolCreateWithString("$context"), (FObject *)context);
	FObjectSetVariable(compiler, FSymbolCreateWithString("$module"), (FObject *)module);
	FObjectSetVariable(compiler, FSymbolCreateWithString("$builder"), (FObject *)LLVMCreateBuilderInContext(context));
	FObjectSetVariable(compiler, FSymbolCreateWithString("$executor"), (FObject *)executor);
	
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


LLVMValueRef FCompilerVisitMessageWithVisitedReceiverAndArguments(FObject *self, FObject *selector, FObject *message, LLVMValueRef receiver, LLVMValueRef arguments[]) {
	LLVMValueRef result = NULL;
	size_t count = FListNodeGetCount(FSend(message, arguments)), paramIndex = 0;
	LLVMBuilderRef builder = FCompilerGetBuilder(FObjectGetPrototype(self));
	if(
		!receiver
	&&	count == 0
	&&	((paramIndex = FFunctionGetIndexOfArgument(FSend(self, function), FSend(message, selector))) != FFunctionArgumentNotFound)
	) {
		result = LLVMGetParam(LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder)), paramIndex + 2);
	} else {
		LLVMValueRef parameters[count + 2];
		parameters[0] = receiver ?: FCompilerGetReferenceToObject(self, FSend(self, context));
		parameters[1] = FCompilerGetReferenceToObject(self, FSend(message, selector));
		size_t index = 0;
		for(index = 0; index < count; index++) {
			parameters[index + 2] = arguments[index];
		}
		
		LLVMValueRef method = LLVMBuildCall(builder, FCompilerGetMethodFunction(FObjectGetPrototype(self)), arguments, 2, "");
		LLVMValueRef implementation = LLVMBuildCall(builder, FCompilerGetImplementationFunction(FObjectGetPrototype(self)), (LLVMValueRef[]){ method }, 1, "get fptr");
		result = LLVMBuildCall(builder, LLVMBuildPointerCast(builder, implementation, LLVMPointerType(FCompilerGetMethodTypeOfArity(FObjectGetPrototype(self), FSymbolGetArity(FSend(message, selector))), 0), "cast fptr to method type"), arguments, count + 2, FSymbolGetString(FSend(message, selector)));
	}
	return result;
}

FObject *FCompilerVisitFunction(FObject *self, FObject *selector, FObject *function) {
	
	return NULL;
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
	FObject *visitor = FObjectCreate(compiler);
	FObjectSetVariable(visitor, FSymbolCreateWithString("function"), function);
	FObjectSetVariable(visitor, FSymbolCreateWithString("context"), context);
	FObjectSetMethod(visitor, FSymbolCreateWithString("function"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(visitor, FSymbolCreateWithString("context"), FFunctionCreateWithImplementation(NULL, (FImplementation)FObjectGetVariable));
	
	FObject *messageNode = FSend(function, messages);
	while(messageNode) {
		result = (LLVMValueRef)FSend(FSend(messageNode, object), acceptVisitor:, visitor);
		messageNode = FSend(messageNode, next);
	}
	
	LLVMBuildRet(FCompilerGetBuilder(compiler), result);
	
	LLVMVerifyFunction(f, LLVMAbortProcessAction);
	// optimize the function
	
	return LLVMRecompileAndRelinkFunction(FCompilerGetExecutor(compiler), f);
}
