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


task :test => [
	
]