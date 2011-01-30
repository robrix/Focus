$: << "External/hax/lib"

require "hax"

libfocus = Hax::Target.new("libfocus") do |t|
	t.compile_sources Dir["Core/**/*.c"]
	t.compile_phase.other_flags = %w("-Wno-unknown-pragmas")
	
	t.link_phase.product_type = Hax::StaticArchiveProductType.new
	t.link_phase.product_name = "libfocus.a"
end

focus = Hax::Target.new("focus") do |t|
	t.compile_sources Dir["Driver/*.c"]
	t.compile_phase.other_flags = %w("-Wno-unknown-pragmas")
	t.search_for_user_headers_in "Core"
	
	t.depends_on libfocus
	t.link_with_target libfocus
	
	t.search_for_libraries_in %x{llvm-config --libdir}.chomp
	t.link_with *%x{llvm-config --libnames engine}.split(" ").collect{ |lib| lib[/lib(.+?)\.a/, 1] }
	t.link_phase.linker = `which clang++`.chomp
end

tests = Hax::Target.new("tests") do |t|
	t.compile_sources Dir["Tests/*.c"]
	t.compile_phase.other_flags = %w("-Wno-unknown-pragmas")
	
	t.depends_on libfocus
	t.link_with_target libfocus
	
	t.search_for_libraries_in %x{llvm-config --libdir}.chomp
	t.link_with *%x{llvm-config --libnames engine}.split(" ").collect{ |lib| lib[/lib(.+?)\.a/, 1] }
	t.link_phase.linker = `which clang++`.chomp
	
	t.build_phase("Run tests") do |target|
		puts "Running tests.\n\n"
		puts %x{#{target.product_path} #{ENV['SUITE']}}
		raise "Tests failed." unless $?.success?
	end
end

task :default => :tests
