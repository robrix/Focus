// FParserTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/Parser/FParser.h"
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

static void testParsesSelectorFragments() {
	size_t length = 0;
	FAssert(FParseSelectorFragment("foo:", 0, &length) && length == 4);
	
	length = 0;
	FAssert(FParseSelectorFragment("foo:4", 0, &length) && length == 4);
	
	length = 0;
	FAssert(FParseSelectorFragment(":1", 0, &length) && length == 1);
	
	
	length = 0;
	FAssert(FParseSelectorFragment("foo", 0, &length) == 0 && length == 0);
	
	length = 0;
	FAssert((FParseSelectorFragment("", 0, &length) == 0) && length == 0);
	
	length = 0;
	FAssert((FParseSelectorFragment("1", 0, &length) == 0) && length == 0);
}

void FRunParserTests() {
	FRunTestSuite("FParser", NULL, NULL, (FTestSuiteTestFunction[]){
		testParsesCharacterSets,
		testParsesTokens,
		
		testParsesWords,
		testParsesWhitespace,
		testParsesWhitespaceAndNewlines,
		
		testParsesSelectorFragments,
		NULL,
	});
}
