$: << "External/hax/lib"

require "hax"

focus = Hax::Target.new("focus") do |t|
	t.compile_sources Dir["Core/**/*.c"]
	t.compile_phase.other_flags = %w("-Wno-unknown-pragmas")
	
	t.link_phase.product_type = Hax::StaticArchiveProductType.new
end

tests = Hax::Target.new("tests") do |t|
	t.compile_sources Dir["Tests/*.c"]
	t.compile_phase.other_flags = %w("-Wno-unknown-pragmas")
	
	t.depends_on focus
	t.link_with_target focus
	
	t.search_for_libraries_in %x{llvm-config --libdir}.chomp
	t.link_with *%x{llvm-config --libnames engine}.split(" ").collect{ |lib| lib[/lib(.+?)\.a/, 1] }
	t.link_phase.linker = `which clang++`.chomp
	
	t.build_phase("Run tests") do |target|
		puts "Running tests.\n\n"
		puts %x{#{target.product_path}}
		raise "Tests failed." unless $?.success?
	end
end

task :default => :tests
