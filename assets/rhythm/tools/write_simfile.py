import json
import os
import struct
import sys

# This needs to be kep in sync with SimfileEventType
type_map = {
	"none": 0,
	"tap": 1,
	"hold_start": 2,
	"hold_end": 3,
	"roll_start": 4,
	"roll_end": 5,
	"mine": 6
}

input_path = sys.argv[1]
print ("Processing: {}".format(input_path))

with open(input_path, "r") as json_file:
	simfile = json.load(json_file)

output_path = os.path.splitext(input_path)[0] + ".csm"
print ("Writing: {}".format(output_path))

# ">" big endian  "<" little endian
endian = ">"

with open(output_path, "wb") as output_file:
	#write header
	header_data = struct.pack('{}ffI'.format(endian), simfile["duration"], simfile["time_offset"], len(simfile["events"]))
	output_file.write(header_data)

	for event in simfile["events"]:
		
		event_type = type_map[event["type"]]
		column_mask = 0
		for col in event["columns"]:
			column_mask |= 1 << col

		# this format string needs to correspond with SimfileEvent
		event_data = struct.pack("{}fHH".format(endian), event["time"], event_type, column_mask);

		output_file.write(event_data)