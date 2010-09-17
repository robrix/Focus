require "rake/clean"

CLEAN << "build"

directory "build/objects"
directory "build/products"
directory "build/generated"

def compile(source, object)
	%x{cc -Wall "#{source}" -I "#{File.dirname source}" -c -o "build/objects/#{object}"}
end

def link(program, objects)
	%x{cc -o "build/products/#{program}" #{objects.join(" ")}}
end

BUILD_TARGETS = []
TEST_TARGETS = []
Dir["Targets/*.rb"].each do |target|
	name = File.basename(target, ".rb")
	namespace name do
		require target
		
		directory "build/objects/#{name}"
		
		rule '.o' => ['.c', "build/objects/#{name}"] do |t|
			compile(t.source, name + "/" + File.basename(t.name))
		end
		
		task :build => ["build/products"] do |t|
			link(name, t.prerequisites.select{ |obj| obj[/\.o$/] }.collect{ |obj| "build/objects/#{name}/#{File.basename(obj)}" })
		end
		
		task :test => ["build/products"] do |t|
			link("test-#{name}", t.prerequisites.select{ |obj| obj[/\.o$/] }.collect{ |obj| "build/objects/#{name}/#{File.basename(obj)}" })
			sh %Q{build/products/test-#{name}}
		end
	end
	BUILD_TARGETS << "#{name}:build"
	TEST_TARGETS << "#{name}:test"
end

desc "Builds all targets."
task :"build-all" => BUILD_TARGETS
task :build => [:"build-all"]
task :default => :build

desc "Tests all targets."
task :"test-all" => TEST_TARGETS
task :test => [:"test-all"]
