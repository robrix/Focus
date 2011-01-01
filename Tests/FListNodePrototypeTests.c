// FListNodePrototypeTests.c
// Created by Rob Rix on 2010-11-22
// Copyright 2010 Monochrome Industries

#include "Core/Prototypes/FListNodePrototype.h"
#include "Core/Prototypes/FObjectPrototype.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "FTestSuite.h"

static FObject *ListNode = NULL;
static FObject *Object = NULL;
static void setUp() {
	ListNode = FSend(FSend(FTestEvaluator, Context), ListNode);
	Object = FSend(FSend(FTestEvaluator, Context), Object);
}


static void testInheritsFromObject() {
	FAssert(FObjectGetPrototype(ListNode) == Object);
}


static void testNewInstancesInheritFromThePrototype() {
	FObject *instance = FSend(ListNode, new);
	FAssert(instance != NULL);
	FAssert(FObjectGetPrototype(instance) == ListNode);
}

static void testInstancesCanBeCreatedWithAnObject() {
	FObject *instance = FSend(ListNode, newWithObject:, Object);
	FAssert(FSend(instance, object) == Object);
	FAssert(FObjectGetPrototype(instance) == ListNode);
}


static void testInstancesReferToTheNextNode() {
	FObject
		*next = FSend(ListNode, newWithObject:, FObjectPrototypeGet()),
		*instance = FSend(ListNode, newWithObject:nextNode:, FObjectPrototypeGet(), next);
	FAssert(FSend(instance, next) == next);
	FAssert(FObjectGetPrototype(instance) == ListNode);
}

static void testCanFetchTheLastNode() {
	FObject
		*unary = FSend(ListNode, newWithObject:, Object),
		*binary = FSend(ListNode, newWithObject:nextNode:, Object, unary),
		*ternary = FSend(ListNode, newWithObject:nextNode:, Object, binary);
	
	FAssert(FSend(ternary, last) == unary);
	FAssert(FSend(binary, last) == unary);
	FAssert(FSend(unary, last) == unary);
}


static void testCanBeConvenientlyCreatedFromCCode() {
	FObject *list = FListNodeCreateWithObjects(FSend(FTestEvaluator, Context), Object, Object, Object, NULL);
	
	FAssert(FSend(list, object) == Object);
	FAssert(FSend(FSend(list, next), object) == Object);
	FAssert(FSend(FSend(FSend(list, next), next), object) == Object);
	FAssert(FSend(FSend(FSend(list, next), next), next) == NULL);
}


void FRunListNodePrototypeTests() {
	FRunTestSuite("FListNodePrototype", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testInheritsFromObject),
		
		FTestCase(testNewInstancesInheritFromThePrototype),
		FTestCase(testInstancesCanBeCreatedWithAnObject),
		
		FTestCase(testInstancesReferToTheNextNode),
		
		FTestCase(testCanFetchTheLastNode),
		
		FTestCase(testCanBeConvenientlyCreatedFromCCode),
		
		{0},
	});
}
