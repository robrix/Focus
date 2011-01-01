// FFunctionPrototypeTests.c
// Created by Rob Rix on 2010-11-10
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "Core/Prototypes/FListNodePrototype.h"
#include "FTestSuite.h"

static void testCompilesLazily() {
	FObject *receiver = FObjectCreate(NULL);
	FObject *context = FSend(FTestEvaluator, Context);
	FObject *message = FSend(FSend(context, Message), newWithReceiver:selector:arguments:, NULL, FSymbolCreateWithString("Object"), NULL);
	FObject *messages = FSend(FSend(context, ListNode), newWithObject:, message);
	FObject *function = FSend(FSend(context, Function), newWithContext:arguments:messages:, context, NULL, messages);
	
	FObjectSetMethod(receiver, FSymbolCreateWithString("test"), function);
	FAssert(FSend(receiver, test) == FSend(context, Object));
}


void FRunFunctionPrototypeTests() {
	FRunTestSuite("FFunctionPrototype", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testCompilesLazily),
		
		{0},
	});
}
