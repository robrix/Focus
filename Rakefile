require "rake/clean"

CLEAN << "build"

directory "build/objects"

rule '.o' => ['.c', 'build/objects'] do |t|
	%x{cc -Wall "#{t.source}" -c -o "build/objects/#{File.basename t.name}"}
end

BUILD_TARGETS = []
TEST_TARGETS = []
Dir["Targets/*.rb"].each do |target|
	name = File.basename(target, ".rb")
	namespace name do
		require target
	end
	BUILD_TARGETS << "#{name}:build"
	TEST_TARGETS << "#{name}:test"
end

desc "Builds all targets."
task :"build-all" => BUILD_TARGETS
task :build => [:"build-all"]
task :default => :build
