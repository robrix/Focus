Target.new "libfocus" do |t|
	t.sources = Dir["Core/*.c"]
	t.compiler = `which clang`
	t.type = :static_archive
end
