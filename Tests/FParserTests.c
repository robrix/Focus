// FParserTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/FObject.h"
#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "Core/FParser.h"
#include "Core/FFunction.h"
#include "Core/FSymbol.h"
#include "FTestSuite.h"

static FObject *FParserTestsContext = NULL;

typedef struct FMessageFixture { const char *source; size_t length; } FMessageFixture;

static void setUp() {
	FParserTestsContext = FObjectCreate(FContextPrototypeGet());
}

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
	FObject *message = NULL;
	FAssert(FParseMessage(NULL, FParserTestsContext, "foo", 0, &length, &message) && length == 3);
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("foo")));
		FAssert(FSend(message, receiver) == NULL);
		FAssert(FSend(message, context) == FParserTestsContext);
	}
}

static void testParsesUnaryMessages() {
	size_t length = 0;
	FObject *message = NULL;
	FAssert(FParseMessage(NULL, FParserTestsContext, "foo: bar", 0, &length, &message) && length == 8);
	
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("foo:")));
		FAssert(FSend(message, context) == FParserTestsContext);
		FObject *argument = FSend(FSend(message, arguments), object);
		if(FAssert(argument != NULL)) {
			FAssert(FSymbolIsEqual((FSymbol *)FSend(argument, selector), FSymbolCreateWithString("bar")));
			FAssert(FSend(argument, arguments) == NULL);
			FAssert(FSend(argument, context) == FSend(message, context));
			FAssert(FSend(argument, receiver) == NULL);
		}
	}
}

static void testParsesBinaryMessages() {
	size_t length = 0;
	FObject *message = NULL;
	FAssert(FParseMessage(NULL, FParserTestsContext, "foo: (bar) quux: (thing)", 0, &length, &message) && length == 24);
	
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("foo:quux:")));
		FAssert(FSend(message, context) == FParserTestsContext);
		FAssert(FSend(message, receiver) == NULL);
		
		FObject
			*fooArgument = FSend(FSend(message, arguments), object),
			*quuxArgument = FSend(FSend(FSend(message, arguments), next), object);
		
		FAssert(FSymbolIsEqual((FSymbol *)FSend(fooArgument, selector), FSymbolCreateWithString("bar")));
		FAssert(FSend(fooArgument, arguments) == NULL);
		FAssert(FSend(fooArgument, context) == FParserTestsContext);
		FAssert(FSend(fooArgument, receiver) == NULL);
		
		FAssert(FSymbolIsEqual((FSymbol *)FSend(quuxArgument, selector), FSymbolCreateWithString("thing")));
		FAssert(FSend(quuxArgument, arguments) == NULL);
		FAssert(FSend(quuxArgument, context) == FParserTestsContext);
		FAssert(FSend(quuxArgument, receiver) == NULL);
	}
}


static void testParsesNullaryMessageChains() {
	FMessageFixture fixtures[] = {
		{"foo bar quux", 12},
		{"((foo) bar) quux", 16},
	};
	for(unsigned i = 0; i < 2; i++) {
		size_t length = 0;
		FObject *message = NULL;
		// printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("quux")));
			FAssert(FSend(message, arguments) == NULL);
			FAssert(FSend(message, context) == FParserTestsContext);
			if(FAssert((message = FSend(message, receiver)) != NULL)) {
				FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("bar")));
				FAssert(FSend(message, arguments) == NULL);
				FAssert(FSend(message, context) == FParserTestsContext);
				if(FAssert((message = FSend(message, receiver)) != NULL)) {
					FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("foo")));
					FAssert(FSend(message, arguments) == NULL);
					FAssert(FSend(message, context) == FParserTestsContext);
					FAssert(FSend(message, receiver) == NULL);
				}
			}
		}
	}
}

static void testParsesUnaryMessagesAfterNullaryMessageChains() {
	FMessageFixture fixtures[] = {
		{"foo bar: quux", 13},
		{"(foo) bar: (quux)", 17},
	};
	for(unsigned i = 0; i < 2; i++) {
		size_t length = 0;
		FObject *message = NULL;
		// printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("bar:")));
			FAssert(FSend(message, context) == FParserTestsContext);
			if(FAssert(FSend(message, receiver) != NULL)) {
				FAssert(FSymbolIsEqual((FSymbol *)FSend(FSend(message, receiver), selector), FSymbolCreateWithString("foo")));
				FAssert(FSend(FSend(message, receiver), arguments) == NULL);
				FAssert(FSend(FSend(message, receiver), context) == FParserTestsContext);
				FAssert(FSend(FSend(message, receiver), receiver) == NULL);
			}
			if(FAssert(FSend(message, arguments) != NULL)) {
				if(FAssert(FSend(FSend(message, arguments), object) != NULL)) {
					FAssert(FSymbolIsEqual((FSymbol *)FSend(FSend(FSend(message, arguments), object), selector), FSymbolCreateWithString("quux")));
					FAssert(FSend(FSend(FSend(message, arguments), object), context) == FParserTestsContext);
					FAssert(FSend(FSend(FSend(message, arguments), object), receiver) == NULL);
					FAssert(FSend(FSend(FSend(message, arguments), object), arguments) == NULL);
				}
			}
		}
	}
}

static void testParsesNullaryMessageChainsAsArguments() {
	FMessageFixture fixtures[] = {
		{"foo bar: quux thing", 19},
		{"(foo) bar: (quux thing)", 23},
	};
	for(unsigned i = 0; i < 2; i++) {
		size_t length = 0;
		FObject *message = NULL;
		// printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("bar:")));
			FAssert(FSend(message, context) == FParserTestsContext);
			FObject *foo = FSend(message, receiver);
			if(FAssert(foo != NULL)) {
				FAssert(FSymbolIsEqual((FSymbol *)FSend(foo, selector), FSymbolCreateWithString("foo")));
				FAssert(FSend(foo, arguments) == NULL);
				FAssert(FSend(foo, context) == FParserTestsContext);
				FAssert(FSend(foo, receiver) == NULL);
			}
			if(FAssert(FSend(message, arguments) != NULL)) {
				FObject *thing = FSend(FSend(message, arguments), object);
				if(FAssert(thing != NULL)) {
					FAssert(FSymbolIsEqual((FSymbol *)FSend(thing, selector), FSymbolCreateWithString("thing")));
					FAssert(FSend(thing, context) == FParserTestsContext);
					FAssert(FSend(thing, arguments) == NULL);
					FObject *quux = FSend(thing, receiver);
					if(FAssert(quux != NULL)) {
						FAssert(FSymbolIsEqual((FSymbol *)FSend(quux, selector), FSymbolCreateWithString("quux")));
						FAssert(FSend(quux, context) == FParserTestsContext);
						FAssert(FSend(quux, arguments) == NULL);
						FAssert(FSend(quux, receiver) == NULL);
					}
				}
			}
		}
	}
}

static void testParsesMessagesChainedOntoNullaryMessageChains() {
	FMessageFixture fixtures[] = {
		{"foo bar quux: thing", 19},
		{"((foo) bar) quux: (thing)", 25},
	};
	for(unsigned i = 0; i < 2; i++) {
		size_t length = 0;
		FObject *message = NULL;
		// printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual((FSymbol *)FSend(message, selector), FSymbolCreateWithString("quux:")));
			FAssert(FSend(message, context) == FParserTestsContext);
			FObject *bar = FSend(message, receiver);
			if(FAssert(bar != NULL)) {
				FAssert(FSymbolIsEqual((FSymbol *)FSend(bar, selector), FSymbolCreateWithString("bar")));
				FAssert(FSend(bar, arguments) == NULL);
				FAssert(FSend(bar, context) == FParserTestsContext);
				FObject *foo = FSend(bar, receiver);
				if(FAssert(foo != NULL)) {
					FAssert(FSymbolIsEqual((FSymbol *)FSend(foo, selector), FSymbolCreateWithString("foo")));
					FAssert(FSend(foo, arguments) == NULL);
					FAssert(FSend(foo, context) == FParserTestsContext);
					FAssert(FSend(foo, receiver) == NULL);
				}
			}
			if(FAssert(FSend(message, arguments) != NULL)) {
				FObject *thing = FSend(FSend(message, arguments), object);
				if(FAssert(thing != NULL)) {
					FAssert(FSymbolIsEqual((FSymbol *)FSend(thing, selector), FSymbolCreateWithString("thing")));
					FAssert(FSend(thing, context) == FParserTestsContext);
					FAssert(FSend(thing, arguments) == NULL);
					FAssert(FSend(thing, receiver) == NULL);
				}
			}
		}
	}
}


static void testParsesParameters() {
	size_t length = 0;
	FSymbol *symbol = NULL;
	FAssert(FParseParameter("foo", 0, &length, &symbol) && length == 3 && FSymbolIsEqual(symbol, FSymbolCreateWithString("foo")));
}

static void testParsesParameterLists() {
	size_t length = 0;
	FObject *node = NULL;
	FAssert(FParseParameterList("foo", 0, &length, &node) && length == 3 && FSymbolIsEqual((FSymbol *)FSend(node, object), FSymbolCreateWithString("foo")));
	FAssert(
		FParseParameterList("foo, bar", 0, &length, &node)
	&&	length == 8
	&&	FSymbolIsEqual((FSymbol *)FSend(node, object), FSymbolCreateWithString("foo"))
	&&	FSymbolIsEqual((FSymbol *)FSend(FSend(node, next), object), FSymbolCreateWithString("bar"))
	);
	
	FAssert(!FParseParameterList("", 0, NULL, NULL));
	FAssert(!FParseParameterList("foo,", 0, NULL, NULL));
}

static void testParsesNAryFunctions() {
	size_t length = 0;
	FObject *function = NULL;
	FAssert(
		FParseNAryFunction("{x->x}", 0, &length, &function)
	&&	length == 6
	&&	FSymbolIsEqual((FSymbol *)FSend(FSend(function, arguments), object), FSymbolCreateWithString("x"))
	&&	FSymbolIsEqual((FSymbol *)FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("x"))
	);
}

static void testParsesNullaryFunctions() {
	size_t length = 0;
	FObject *function = NULL;
	FAssert(
		FParseNullaryFunction("{foo}", 0, &length, &function)
	&&	length == 5
	&&	FSend(function, arguments) == NULL
	&&	FSymbolIsEqual((FSymbol *)FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("foo"))
	);
	FAssert(
		FParseNullaryFunction("{ foo }", 0, &length, &function)
	&&	length == 7
	&&	FSend(function, arguments) == NULL
	&&	FSymbolIsEqual((FSymbol *)FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("foo"))
	);
}


static void testParsesParenthesizedExpressions() {
	size_t length = 0;
	FAssert(FParseParenthesizedExpression(FParserTestsContext, "(foo)", 0, &length, NULL) && length == 5);
	
	length = 0;
	FAssert(FParseParenthesizedExpression(FParserTestsContext, "( foo ) ", 0, &length, NULL) && length == 7);
	
	length = 0;
	FAssert(FParseParenthesizedExpression(FParserTestsContext, "(\nfoo\n) ", 0, &length, NULL) && length == 7);
}

static void testParsesExpressions() {
	size_t length = 0;
	FAssert(FParseExpression(FParserTestsContext, "(\nfoo\n)", 0, &length, NULL) && length == 7);
	
	length = 0;
	FAssert(FParseExpression(FParserTestsContext, "foo", 0, &length, NULL) && length == 3);
}

static void testParsesExpressionLists() {
	size_t length = 0;
	FObject *node = NULL;
	FAssert(FParseExpressionList(FParserTestsContext, "foo", 0, &length, &node) && length == 3 && node != NULL && FSend(node, next) == NULL);
	FAssert(FParseExpressionList(FParserTestsContext, "foo\n", 0, &length, &node) && length == 3 && node != NULL && FSend(node, next) == NULL);
	FAssert(FParseExpressionList(FParserTestsContext, "foo\nbar", 0, &length, &node) && length == 7 && node != NULL && FSend(node, next) != NULL);
	
	FAssert(!FParseExpressionList(FParserTestsContext, "", 0, &length, &node));
}


void FRunParserTests() {
	FRunTestSuite("FParser", setUp, NULL, (FTestSuiteTestCase[]){
		FTestCase(testParsesCharacterSets),
		FTestCase(testParsesTokens),
		
		FTestCase(testParsesWords),
		FTestCase(testParsesWhitespace),
		FTestCase(testParsesWhitespaceAndNewlines),
		
		FTestCase(testParsesKeywords),
		
		FTestCase(testParsesNullaryMessages),
		FTestCase(testParsesUnaryMessages),
		FTestCase(testParsesBinaryMessages),
		
		FTestCase(testParsesNullaryMessageChains),
		FTestCase(testParsesUnaryMessagesAfterNullaryMessageChains),
		FTestCase(testParsesNullaryMessageChainsAsArguments),
		FTestCase(testParsesMessagesChainedOntoNullaryMessageChains),
		
		FTestCase(testParsesParameters),
		FTestCase(testParsesParameterLists),
		
		FTestCase(testParsesNAryFunctions),
		FTestCase(testParsesNullaryFunctions),
		
		FTestCase(testParsesParenthesizedExpressions),
		FTestCase(testParsesExpressions),
		FTestCase(testParsesExpressionLists),
		
		{0},
	});
}
