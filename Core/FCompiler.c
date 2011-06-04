// FCompiler.c
// Created by Rob Rix on 2010-10-27
// Copyright 2010 Monochrome Industries

#include "FAllocator.h"
#include "FSymbol.h"
#include "FObject+Protected.h"
#include "FCompiler.h"
#include "FEvaluator+Protected.h"
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
	return (LLVMContextRef)FSendMessage(self, FSymbolCreateWithString(" context"));
}

LLVMModuleRef FCompilerGetModule(FObject *self) {
	return (LLVMModuleRef)FSend(self, FSymbolCreateWithString(" module"));
	
}

LLVMBuilderRef FCompilerGetBuilder(FObject *self) {
	return (LLVMBuilderRef)FSend(self, FSymbolCreateWithString(" builder"));
	
}

LLVMExecutionEngineRef FCompilerGetExecutor(FObject *self) {
	return (LLVMExecutionEngineRef)FSend(self, FSymbolCreateWithString(" executor"));
}


LLVMValueRef FCompilerVisitMessageWithVisitedReceiverAndArguments(FObject *self, FSymbol *selector, FObject *message, LLVMValueRef receiver, LLVMValueRef arguments[]);
LLVMValueRef FCompilerVisitFunction(FObject *self, FSymbol *selector, FObject *function);

FObject *FCompilerPrototypeBootstrap(FObject *compiler, FEvaluatorBootstrapState state) {
	static bool initedJIT = 0;
	if(!initedJIT) {
		LLVMLinkInJIT();
		LLVMInitializeNativeTarget();
		initedJIT = 1;
	}
	
	FObjectSetMethod(compiler, FSymbolCreateWithString("visitMessage:withVisitedReceiver:visitedArguments:"), FEvaluatorBootstrapFunction((FImplementation)FCompilerVisitMessageWithVisitedReceiverAndArguments, state));
	FObjectSetMethod(compiler, FSymbolCreateWithString("visitFunction:"), FEvaluatorBootstrapFunction((FImplementation)FCompilerVisitFunction, state));
	
	LLVMContextRef context = LLVMContextCreate();
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("Focus", context);
	LLVMExecutionEngineRef executor = NULL;
	LLVMCreateJITCompilerForModule(&executor, module, 3, NULL);
	FObjectSetMethod(compiler, FSymbolCreateWithString(" context"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(compiler, FSymbolCreateWithString(" module"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(compiler, FSymbolCreateWithString(" builder"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
	FObjectSetMethod(compiler, FSymbolCreateWithString(" executor"), FEvaluatorBootstrapFunction((FImplementation)FObjectGetVariable, state));
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

LLVMTypeRef FCompilerGetSymbolType(FObject *compiler) {
	LLVMTypeRef type = LLVMGetTypeByName(FCompilerGetModule(compiler), "FSymbol");
	if(!type) {
		type = LLVMPointerType(LLVMInt8TypeInContext(FCompilerGetContext(compiler)), 0);
		LLVMAddTypeName(FCompilerGetModule(compiler), "FSymbol", type);
	}
	return type;
}

LLVMValueRef FCompilerGetReferenceToObject(FObject *compiler, FObject *object) {
	return LLVMConstIntToPtr(LLVMConstInt(LLVMInt64TypeInContext(FCompilerGetContext(compiler)), (unsigned long long)object, 0), FCompilerGetObjectType(compiler));
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
	types[0] = FCompilerGetObjectType(compiler);
	types[1] = FCompilerGetSymbolType(compiler);
	for(size_t i = 2; i < arity + 2; i++) {
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


LLVMValueRef FCompilerVisitMessageWithVisitedReceiverAndArguments(FObject *self, FSymbol *selector, FObject *message, LLVMValueRef receiver, LLVMValueRef arguments[]) {
	LLVMValueRef result = NULL;
	size_t count = FListNodeGetCount(FSend(message, arguments)), paramIndex = 0;
	FObject *compiler = FObjectGetPrototype(self);
	LLVMBuilderRef builder = FCompilerGetBuilder(compiler);
	if(
		!receiver
	&&	count == 0
	&&	((paramIndex = FFunctionGetIndexOfArgument(FSend(self, function), FSend(message, selector))) != FFunctionArgumentNotFound)
	) {
		result = LLVMGetParam(LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder)), paramIndex + 2);
	} else {
		LLVMValueRef parameters[count + 2];
		parameters[0] = receiver ?: FCompilerGetReferenceToObject(compiler, FSend(self, context));
		LLVMSetValueName(parameters[0], "receiver");
		parameters[1] = FCompilerGetReferenceToObject(compiler, FSend(message, selector));
		LLVMSetValueName(parameters[1], "selector");
		size_t index = 0;
		for(index = 0; index < count; index++) {
			parameters[index + 2] = arguments[index];
		}
		
		LLVMValueRef method = LLVMBuildCall(builder, FCompilerGetMethodFunction(compiler), parameters, 2, "get function");
		LLVMValueRef implementation = LLVMBuildCall(builder, FCompilerGetImplementationFunction(compiler), (LLVMValueRef[]){ method }, 1, "get implementation");
		LLVMValueRef cast = LLVMBuildPointerCast(builder, implementation, LLVMPointerType(FCompilerGetMethodTypeOfArity(compiler, FSymbolGetArity((FSymbol *)FSend(message, selector))), 0), "cast implementation to specific type");
		result = LLVMBuildCall(builder, cast, parameters, count + 2, FSymbolGetString((FSymbol *)FSend(message, selector)));
	}
	return result;
}

LLVMValueRef FCompilerVisitFunction(FObject *self, FSymbol *selector, FObject *function) {
	#pragma message("fixme: this should create a clone of this function.")
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
	// LLVMSetGC(f, "shadow-stack");
	LLVMAppendBasicBlock(f, "entry");
	LLVMPositionBuilderAtEnd(FCompilerGetBuilder(compiler), LLVMGetEntryBasicBlock(f));
	
	// set up a new context inheriting from the function’s context
	FObject *context = FObjectCreate(FSend(function, context));
	FObject *visitor = FObjectCreate(compiler);
	FObjectSetVariable(visitor, FSymbolCreateWithString("function"), function);
	FObjectSetVariable(visitor, FSymbolCreateWithString("context"), context);
	FObjectSetMethod(visitor, FSymbolCreateWithString("function"), FFunctionCreateWithImplementation(context, (FImplementation)FObjectGetVariable));
	FObjectSetMethod(visitor, FSymbolCreateWithString("context"), FFunctionCreateWithImplementation(context, (FImplementation)FObjectGetVariable));
	
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
