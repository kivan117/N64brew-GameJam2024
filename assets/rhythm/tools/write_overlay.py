import json
import os
import struct
import sys

input_path = sys.argv[1]
print ("Processing: {}".format(input_path))

with open(input_path, "r") as json_file:
	overlay_file = json.load(json_file)

output_path = os.path.splitext(input_path)[0] + ".layout"
print ("Writing: {}".format(output_path))

# ">" big endian  "<" little endian
endian = ">"

with open(output_path, "wb") as output_file:
	#write header
	header_data = struct.pack('{}I'.format(endian), len(overlay_file))
	output_file.write(header_data)

	for overlay_item in overlay_file:
		tex_a = ord(overlay_item["texture"][0])
		tex_b = ord(overlay_item["texture"][1]) if len(overlay_item["texture"]) == 2 else 0
		item_data = struct.pack('{}ffII'.format(endian), overlay_item["x"], overlay_item["y"], tex_a, tex_b)
		output_file.write(item_data)

