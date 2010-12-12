// FParserTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/FObject.h"
#include "Core/Prototypes/FContextPrototype.h"
#include "Core/Prototypes/FMessagePrototype.h"
#include "Core/FParser.h"
#include "Core/Prototypes/FFunctionPrototype.h"
#include "Core/FSymbol.h"
#include "FTestSuite.h"

typedef struct FMessageFixture { const char *source; size_t length; } FMessageFixture;

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
	FAssert(FParseMessage(NULL, "foo", 0, &length, &message) && length == 3);
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("foo")));
		FAssert(FSend(message, receiver) == NULL);
	}
}

static void testParsesUnaryMessages() {
	size_t length = 0;
	FObject *message = NULL;
	FAssert(FParseMessage(NULL, "foo: bar", 0, &length, &message) && length == 8);
	
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("foo:")));
		FAssert(FSend(message, receiver) == NULL);
		FObject *argument = FSend(FSend(message, arguments), object);
		if(FAssert(argument != NULL)) {
			FAssert(FSymbolIsEqual(FSend(argument, selector), FSymbolCreateWithString("bar")));
			FAssert(FSend(argument, arguments) == NULL);
			FAssert(FSend(argument, receiver) == NULL);
		}
	}
}

static void testParsesBinaryMessages() {
	size_t length = 0;
	FObject *message = NULL;
	FAssert(FParseMessage(NULL, "foo: (bar) quux: (thing)", 0, &length, &message) && length == 24);
	
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("foo:quux:")));
		FAssert(FSend(message, receiver) == NULL);
		
		FObject
			*fooArgument = FSend(FSend(message, arguments), object),
			*quuxArgument = FSend(FSend(FSend(message, arguments), next), object);
		
		FAssert(FSymbolIsEqual(FSend(fooArgument, selector), FSymbolCreateWithString("bar")));
		FAssert(FSend(fooArgument, arguments) == NULL);
		FAssert(FSend(fooArgument, receiver) == NULL);
		
		FAssert(FSymbolIsEqual(FSend(quuxArgument, selector), FSymbolCreateWithString("thing")));
		FAssert(FSend(quuxArgument, arguments) == NULL);
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
		FAssert(FParseExpression(fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("quux")));
			FAssert(FSend(message, arguments) == NULL);
			if(FAssert((message = FSend(message, receiver)) != NULL)) {
				FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("bar")));
				FAssert(FSend(message, arguments) == NULL);
				if(FAssert((message = FSend(message, receiver)) != NULL)) {
					FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("foo")));
					FAssert(FSend(message, arguments) == NULL);
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
		FAssert(FParseExpression(fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("bar:")));
			if(FAssert(FSend(message, receiver) != NULL)) {
				FAssert(FSymbolIsEqual(FSend(FSend(message, receiver), selector), FSymbolCreateWithString("foo")));
				FAssert(FSend(FSend(message, receiver), arguments) == NULL);
				FAssert(FSend(FSend(message, receiver), receiver) == NULL);
			}
			if(FAssert(FSend(message, arguments) != NULL)) {
				if(FAssert(FSend(FSend(message, arguments), object) != NULL)) {
					FAssert(FSymbolIsEqual(FSend(FSend(FSend(message, arguments), object), selector), FSymbolCreateWithString("quux")));
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
		FAssert(FParseExpression(fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("bar:")));
			FObject *foo = FSend(message, receiver);
			if(FAssert(foo != NULL)) {
				FAssert(FSymbolIsEqual(FSend(foo, selector), FSymbolCreateWithString("foo")));
				FAssert(FSend(foo, arguments) == NULL);
				FAssert(FSend(foo, receiver) == NULL);
			}
			if(FAssert(FSend(message, arguments) != NULL)) {
				FObject *thing = FSend(FSend(message, arguments), object);
				if(FAssert(thing != NULL)) {
					FAssert(FSymbolIsEqual(FSend(thing, selector), FSymbolCreateWithString("thing")));
					FAssert(FSend(thing, arguments) == NULL);
					FObject *quux = FSend(thing, receiver);
					if(FAssert(quux != NULL)) {
						FAssert(FSymbolIsEqual(FSend(quux, selector), FSymbolCreateWithString("quux")));
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
		FAssert(FParseExpression(fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(FSend(message, selector), FSymbolCreateWithString("quux:")));
			FObject *bar = FSend(message, receiver);
			if(FAssert(bar != NULL)) {
				FAssert(FSymbolIsEqual(FSend(bar, selector), FSymbolCreateWithString("bar")));
				FAssert(FSend(bar, arguments) == NULL);
				FObject *foo = FSend(bar, receiver);
				if(FAssert(foo != NULL)) {
					FAssert(FSymbolIsEqual(FSend(foo, selector), FSymbolCreateWithString("foo")));
					FAssert(FSend(foo, arguments) == NULL);
					FAssert(FSend(foo, receiver) == NULL);
				}
			}
			if(FAssert(FSend(message, arguments) != NULL)) {
				FObject *thing = FSend(FSend(message, arguments), object);
				if(FAssert(thing != NULL)) {
					FAssert(FSymbolIsEqual(FSend(thing, selector), FSymbolCreateWithString("thing")));
					FAssert(FSend(thing, arguments) == NULL);
					FAssert(FSend(thing, receiver) == NULL);
				}
			}
		}
	}
}


static void testParsesParameters() {
	size_t length = 0;
	FObject *symbol = NULL;
	FAssert(FParseParameter("foo", 0, &length, &symbol) && length == 3 && FSymbolIsEqual(symbol, FSymbolCreateWithString("foo")));
}

static void testParsesParameterLists() {
	size_t length = 0;
	FObject *node = NULL;
	FAssert(FParseParameterList("foo", 0, &length, &node) && length == 3 && FSymbolIsEqual(FSend(node, object), FSymbolCreateWithString("foo")));
	FAssert(
		FParseParameterList("foo, bar", 0, &length, &node)
	&&	length == 8
	&&	FSymbolIsEqual(FSend(node, object), FSymbolCreateWithString("foo"))
	&&	FSymbolIsEqual(FSend(FSend(node, next), object), FSymbolCreateWithString("bar"))
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
	&&	FSymbolIsEqual(FSend(FSend(function, arguments), object), FSymbolCreateWithString("x"))
	&&	FSend(FSend(function, arguments), next) == NULL
	&&	FSymbolIsEqual(FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("x"))
	&&	FSend(FSend(function, messages), next) == NULL
	);
	FAssert(
		FParseNAryFunction("{ x -> x }", 0, &length, &function)
	&&	length == 10
	&&	FSymbolIsEqual(FSend(FSend(function, arguments), object), FSymbolCreateWithString("x"))
	&&	FSend(FSend(function, arguments), next) == NULL
	&&	FSymbolIsEqual(FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("x"))
	&&	FSend(FSend(function, messages), next) == NULL
	);
	FAssert(
		FParseNAryFunction("{ x, y -> x\ny }", 0, &length, &function)
	&&	length == 15
	&&	FSymbolIsEqual(FSend(FSend(function, arguments), object), FSymbolCreateWithString("x"))
	&&	FSymbolIsEqual(FSend(FSend(FSend(function, arguments), next), object), FSymbolCreateWithString("y"))
	&&	FSend(FSend(FSend(function, arguments), next), next) == NULL
	&&	FSymbolIsEqual(FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("x"))
	&&	FSymbolIsEqual(FSend(FSend(FSend(FSend(function, messages), next), object), selector), FSymbolCreateWithString("y"))
	&&	FSend(FSend(FSend(function, messages), next), next) == NULL
	);
}

static void testParsesNullaryFunctions() {
	size_t length = 0;
	FObject *function = NULL;
	FAssert(
		FParseNullaryFunction("{foo}", 0, &length, &function)
	&&	function != NULL
	&&	length == 5
	&&	FSend(function, arguments) == NULL
	&&	FSymbolIsEqual(FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("foo"))
	);
	FAssert(
		FParseNullaryFunction("{ foo }", 0, &length, &function)
	&&	length == 7
	&&	function != NULL
	&&	FSend(function, arguments) == NULL
	&&	FSymbolIsEqual(FSend(FSend(FSend(function, messages), object), selector), FSymbolCreateWithString("foo"))
	);
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

static void testParsesExpressionLists() {
	size_t length = 0;
	FObject *node = NULL;
	FAssert(FParseExpressionList("foo", 0, &length, &node) && length == 3 && node != NULL && FSend(node, next) == NULL);
	FAssert(FParseExpressionList("foo\n", 0, &length, &node) && length == 3 && node != NULL && FSend(node, next) == NULL);
	FAssert(FParseExpressionList("foo\nbar", 0, &length, &node) && length == 7 && node != NULL && FSend(node, next) != NULL);
	
	FAssert(!FParseExpressionList("", 0, &length, &node));
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
