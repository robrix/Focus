$: << "External/hax/lib"

require "hax"

libfocus = Hax::Target.new("libfocus") do |t|
	t.sources = Dir["Core/*.c"]
	
	t.product_type = Hax::StaticArchiveProductType.new
end

tests = Hax::Target.new("tests") do |t|
	t.sources = Dir["Tests/*.c"]
	
	t.depends_on libfocus
	t.link_with libfocus
end
