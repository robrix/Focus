// FParserTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/FParser.h"
#include "FTestSuite.h"

static void testParsesWords() {
	size_t length = 0;
	FAssert(FParseWord("_word123", 0, &length) && length == 8);
	
	length = 0;
	FAssert(FParseWord("_word123 ", 0, &length) && length == 8);
	
	length = 0;
	FAssert(FParseWord("_ ", 0, &length) && length == 1);
	
	length = 0;
	FAssert(FParseWord(" _word123", 0, &length) == 0 && length == 0);
	
	length = 0;
	FAssert(FParseWord("123", 0, &length) == 0 && length == 0);
	
	length = 0;
	FAssert(FParseWord("", 0, &length) == 0 && length == 0);
}

static void testParsesTokens() {
	FAssert(FParseToken(":", 0, ":"));
	FAssert(FParseToken(" :", 1, ":"));
	FAssert(FParseToken(":", 1, ":") == 0);
	FAssert(FParseToken(" :", 0, ":") == 0);
	FAssert(FParseToken(":", 2, ":") == 0);
}

static void testParsesCharacterSets() {
	size_t length = 0;
	FAssert(FParseCharacterSet("123", 0, "1234567890", &length) && length == 3);
	
	length = 0;
	FAssert(FParseCharacterSet("123 ", 1, "1234567890", &length) && length == 2);
	
	length = 0;
	FAssert((FParseCharacterSet("123 ", 3, "1234567890", &length) == 0) && length == 0);
}

static void testParsesWhitespace() {
	size_t length = 0;
	FAssert(FParseWhitespace(" \t", 0, &length) && length == 2);
	
	length = 0;
	FAssert(FParseWhitespace(" \t\n\r", 0, &length) && length == 2);
	
	length = 0;
	FAssert((FParseWhitespace(" \n\r\t", 4, &length) == 0) && length == 0);
	
	length = 0;
	FAssert((FParseWhitespace(" \n\r\t1", 4, &length) == 0) && length == 0);
}

static void testParsesWhitespaceAndNewlines() {
	size_t length = 0;
	FAssert(FParseWhitespaceAndNewlines(" \n\r\t", 0, &length) && length == 4);
	
	length = 0;
	FAssert((FParseWhitespaceAndNewlines(" \n\r\t", 4, &length) == 0) && length == 0);
	
	length = 0;
	FAssert((FParseWhitespaceAndNewlines(" \n\r\t1", 4, &length) == 0) && length == 0);
}

static void testParsesKeywords() {
	size_t length = 0;
	FAssert(FParseKeyword("foo:", 0, &length) && length == 4);
	
	length = 0;
	FAssert(FParseKeyword("foo:4", 0, &length) && length == 4);
	
	length = 0;
	FAssert(FParseKeyword(":1", 0, &length) && length == 1);
	
	length = 0;
	FAssert(FParseKeyword(":", 0, &length) && length == 1);
	
	
	length = 0;
	FAssert(FParseKeyword("foo", 0, &length) == 0 && length == 0);
	
	length = 0;
	FAssert((FParseKeyword("", 0, &length) == 0) && length == 0);
	
	length = 0;
	FAssert((FParseKeyword("1", 0, &length) == 0) && length == 0);
}


static void testParsesNullaryMessages() {
	size_t length = 0;
	FAssert(FParseMessage("foo", 0, &length, NULL) && length == 3);
}

static void testParsesUnaryMessages() {
	size_t length = 0;
	FAssert(FParseMessage("foo: bar", 0, &length, NULL) && length == 8);
}

static void testParsesBinaryMessages() {
	size_t length = 0;
	FAssert(FParseMessage("foo: (bar) quux: (thing)", 0, &length, NULL) && length == 24);
}


static void testParsesMessageChains() {
	size_t length = 0;
	FAssert(FParseExpression("foo bar quux", 0, &length, NULL) && length == 12);
	// fixme: test that it’s equivalent to ((foo) bar) quux
	
	length = 0;
	FAssert(FParseExpression("((foo) bar) quux", 0, &length, NULL) && length == 16);
	
	length = 0;
	FAssert(FParseExpression("foo bar: quux", 0, &length, NULL) && length == 13);
	// fixme: test that it’s equivalent to (foo) bar: (quux)
	
	length = 0;
	FAssert(FParseExpression("(foo) bar: (quux)", 0, &length, NULL) && length == 17);
	
	length = 0;
	FAssert(FParseExpression("foo bar: quux thing", 0, &length, NULL) && length == 19);
	// fixme: test that it’s equivalent to (foo) bar: (quux thing)
	
	length = 0;
	FAssert(FParseExpression("(foo) bar: (quux thing)", 0, &length, NULL) && length == 23);
	
	length = 0;
	FAssert(FParseExpression("foo bar quux: thing", 0, &length, NULL) && length == 19);
	// fixme: test that it’s equivalent to ((foo) bar) quux: (thing)
	
	length = 0;
	FAssert(FParseExpression("((foo) bar) quux: (thing)", 0, &length, NULL) && length == 25);
}


static void testParsesParenthesizedExpressions() {
	size_t length = 0;
	FAssert(FParseParenthesizedExpression("(foo)", 0, &length, NULL) && length == 5);
	
	length = 0;
	FAssert(FParseParenthesizedExpression("( foo ) ", 0, &length, NULL) && length == 7);
	
	length = 0;
	FAssert(FParseParenthesizedExpression("(\nfoo\n) ", 0, &length, NULL) && length == 7);
}

static void testParsesExpressions() {
	size_t length = 0;
	FAssert(FParseExpression("(\nfoo\n)", 0, &length, NULL) && length == 7);
	
	length = 0;
	FAssert(FParseExpression("foo", 0, &length, NULL) && length == 3);
}

void FRunParserTests() {
	FRunTestSuite("FParser", NULL, NULL, (FTestSuiteTestCase[]){
		FTestCase(testParsesCharacterSets),
		FTestCase(testParsesTokens),
		
		FTestCase(testParsesWords),
		FTestCase(testParsesWhitespace),
		FTestCase(testParsesWhitespaceAndNewlines),
		
		FTestCase(testParsesKeywords),
		
		FTestCase(testParsesNullaryMessages),
		FTestCase(testParsesUnaryMessages),
		FTestCase(testParsesBinaryMessages),
		
		FTestCase(testParsesMessageChains),
		
		FTestCase(testParsesParenthesizedExpressions),
		FTestCase(testParsesExpressions),
		
		{0},
	});
}
