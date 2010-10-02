$: << "External/hax/lib"

require "hax"

libfocus = Hax::Target.new("libfocus") do |t|
	t.compile_sources Dir["Core/*.c"]
	t.link_phase.product_type = Hax::StaticArchiveProductType.new
end

tests = Hax::Target.new("tests") do |t|
	t.compile_sources Dir["Tests/*.c"]
	
	t.depends_on libfocus
	t.link_with_target libfocus
	
	t.build_phase("Run tests") do |target|
		puts "Running tests"
		puts %x{#{target.product_path} || echo "Tests failed."}
	end
end

task :default => :tests
