require 'rexml/document'

xml = File.read("sync.rocket")

doc = REXML::Document.new(xml)

trackNames = ""
numKeys = ""
deltaRows = ""
values = ""
interpolations = ""

doc.elements.each('tracks/track') do |track|
	trackNames += "\"" + track.attributes['name'] + "\", "
	
	prevRow = 0
	i = 0
	
	deltaRows += "\n// " + track.attributes['name'] + "\n"
	values += "\n// " + track.attributes['name'] + "\n"
	interpolations += "\n// " + track.attributes['name'] + "\n"
	
	track.elements.each('key') do |key|
		row = key.attributes['row'].to_i
		deltaRows += (row - prevRow).to_s + ", "
		
		values += key.attributes['value'].to_f.to_s + "f, "
		interpolations += key.attributes['interpolation'] + ", "
		
		i += 1
		prevRow = row
	end
	
	numKeys += i.to_s + ", "
end

# puts trackNames
# puts numKeys
# puts deltaRows
# puts values
# puts interpolations

File.open("sync_rocket.cpp", 'w') { |f|
		f.write("#include \"sync_rocket.h\"\n")
		f.write("const char* const trackNames[] = {" + trackNames + "\n};\n")
		f.write("const short numKeys[] = { " + numKeys + "\n};\n")
		f.write("const float values[] = { " + values + "\n};\n")
		f.write("const short deltaRows[] = { " + deltaRows + "\n};\n")
		f.write("const char interpolations[] = { " + interpolations + "\n};\n")
}

File.open("sync_rocket.h", 'w') { |f|
		f.write("const extern char* const trackNames[];\n")
		f.write("const extern short numKeys[];\n")
		f.write("const extern float values[];\n")
		f.write("const extern short deltaRows[];\n")
		f.write("const extern char interpolations[];\n")
}