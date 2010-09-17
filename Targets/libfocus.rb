desc "Builds libfocus."
task :build => [
	"Core/FArray.o",
	"Core/FGarbageCollector.o",
	"Core/FHashTable.o",
	"Core/FInteger.o",
	"Core/FObject.o",
	"Core/FReal.o",
	"Core/FSymbol.o",
]

desc "Tests libfocus."
task :test => [
	"Tests/FHashTableTests.o",
	"Tests/FTestSuite.o",
]