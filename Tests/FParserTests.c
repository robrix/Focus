// FParserTests.c
// Created by Rob Rix on 2010-10-05
// Copyright 2010 Monochrome Industries

#include "Core/FObject.h"
#include "Core/Prototypes/FContextPrototype.h"
#include "Core/FMessage.h"
#include "Core/FParser.h"
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
	FMessage *message = NULL;
	FAssert(FParseMessage(NULL, FParserTestsContext, "foo", 0, &length, &message) && length == 3);
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("foo")));
		FAssert(message->receiver == NULL);
		FAssert(message->context == FParserTestsContext);
	}
}

static void testParsesUnaryMessages() {
	size_t length = 0;
	FMessage *message = NULL;
	FAssert(FParseMessage(NULL, FParserTestsContext, "foo: bar", 0, &length, &message) && length == 8);
	
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("foo:")));
		FAssert(message->context == FParserTestsContext);
		FMessage *argument = message->arguments->message;
		if(FAssert(argument != NULL)) {
			FAssert(FSymbolIsEqual(argument->selector, FSymbolCreateWithString("bar")));
			FAssert(argument->arguments == NULL);
			FAssert(argument->context == message->context);
			FAssert(argument->receiver == NULL);
		}
	}
}

static void testParsesBinaryMessages() {
	size_t length = 0;
	FMessage *message = NULL;
	FAssert(FParseMessage(NULL, FParserTestsContext, "foo: (bar) quux: (thing)", 0, &length, &message) && length == 24);
	
	if(FAssert(message != NULL)) {
		FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("foo:quux:")));
		FAssert(message->context == FParserTestsContext);
		FAssert(message->receiver == NULL);
		
		FMessage
			*fooArgument = message->arguments->message,
			*quuxArgument = message->arguments->nextNode->message;
		
		FAssert(FSymbolIsEqual(fooArgument->selector, FSymbolCreateWithString("bar")));
		FAssert(fooArgument->arguments == NULL);
		FAssert(fooArgument->context == FParserTestsContext);
		FAssert(fooArgument->receiver == NULL);
		
		FAssert(FSymbolIsEqual(quuxArgument->selector, FSymbolCreateWithString("thing")));
		FAssert(quuxArgument->arguments == NULL);
		FAssert(quuxArgument->context == FParserTestsContext);
		FAssert(quuxArgument->receiver == NULL);
	}
}


static void testParsesNullaryMessageChains() {
	FMessageFixture fixtures[] = {
		{"foo bar quux", 12},
		{"((foo) bar) quux", 16},
	};
	for(unsigned i = 0; i < 2; i++) {
		size_t length = 0;
		FMessage *message = NULL;
		printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("quux")));
			FAssert(message->arguments == NULL);
			FAssert(message->context == FParserTestsContext);
			if(FAssert((message = message->receiver) != NULL)) {
				FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("bar")));
				FAssert(message->arguments == NULL);
				FAssert(message->context == FParserTestsContext);
				if(FAssert((message = message->receiver) != NULL)) {
					FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("foo")));
					FAssert(message->arguments == NULL);
					FAssert(message->context == FParserTestsContext);
					FAssert(message->receiver == NULL);
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
		FMessage *message = NULL;
		printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("bar:")));
			FAssert(message->context == FParserTestsContext);
			if(FAssert(message->receiver != NULL)) {
				FAssert(FSymbolIsEqual(message->receiver->selector, FSymbolCreateWithString("foo")));
				FAssert(message->receiver->arguments == NULL);
				FAssert(message->receiver->context == FParserTestsContext);
				FAssert(message->receiver->receiver == NULL);
			}
			if(FAssert(message->arguments != NULL)) {
				if(FAssert(message->arguments->message != NULL)) {
					FAssert(FSymbolIsEqual(message->arguments->message->selector, FSymbolCreateWithString("quux")));
					FAssert(message->arguments->message->context == FParserTestsContext);
					FAssert(message->arguments->message->receiver == NULL);
					FAssert(message->arguments->message->arguments == NULL);
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
		FMessage *message = NULL;
		printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("bar:")));
			FAssert(message->context == FParserTestsContext);
			FMessage *foo = message->receiver;
			if(FAssert(foo != NULL)) {
				FAssert(FSymbolIsEqual(foo->selector, FSymbolCreateWithString("foo")));
				FAssert(foo->arguments == NULL);
				FAssert(foo->context == FParserTestsContext);
				FAssert(foo->receiver == NULL);
			}
			if(FAssert(message->arguments != NULL)) {
				FMessage *thing = message->arguments->message;
				if(FAssert(thing != NULL)) {
					FAssert(FSymbolIsEqual(thing->selector, FSymbolCreateWithString("thing")));
					FAssert(thing->context == FParserTestsContext);
					FAssert(thing->arguments == NULL);
					FMessage *quux = thing->receiver;
					if(FAssert(quux != NULL)) {
						FAssert(FSymbolIsEqual(quux->selector, FSymbolCreateWithString("quux")));
						FAssert(quux->context == FParserTestsContext);
						FAssert(quux->arguments == NULL);
						FAssert(quux->receiver == NULL);
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
		FMessage *message = NULL;
		printf("\t%s\n", fixtures[i].source);
		FAssert(FParseExpression(FParserTestsContext, fixtures[i].source, 0, &length, &message) && length == fixtures[i].length);
		if(FAssert(message != NULL)) {
			FAssert(FSymbolIsEqual(message->selector, FSymbolCreateWithString("quux:")));
			FAssert(message->context == FParserTestsContext);
			FMessage *bar = message->receiver;
			if(FAssert(bar != NULL)) {
				FAssert(FSymbolIsEqual(bar->selector, FSymbolCreateWithString("bar")));
				FAssert(bar->arguments == NULL);
				FAssert(bar->context == FParserTestsContext);
				FMessage *foo = bar->receiver;
				if(FAssert(foo != NULL)) {
					FAssert(FSymbolIsEqual(foo->selector, FSymbolCreateWithString("foo")));
					FAssert(foo->arguments == NULL);
					FAssert(foo->context == FParserTestsContext);
					FAssert(foo->receiver == NULL);
				}
			}
			if(FAssert(message->arguments != NULL)) {
				FMessage *thing = message->arguments->message;
				if(FAssert(thing != NULL)) {
					FAssert(FSymbolIsEqual(thing->selector, FSymbolCreateWithString("thing")));
					FAssert(thing->context == FParserTestsContext);
					FAssert(thing->arguments == NULL);
					FAssert(thing->receiver == NULL);
				}
			}
		}
	}
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
		
		FTestCase(testParsesParenthesizedExpressions),
		FTestCase(testParsesExpressions),
		
		{0},
	});
}
