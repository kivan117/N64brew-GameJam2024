#include <filesystem>
#include <fstream>
#include <iostream>

typedef struct {
	float duration;
	float time_offset;
	uint32_t event_count;
} CompactSimfileHeader;

typedef struct {
    float time;
    uint16_t duration;
    uint8_t type;
    uint8_t columns;
} SimfileEvent;

int main(int argc, char** argv) {
	std::filesystem::path input_path = argv[1];

	if (std::filesystem::exists(input_path)) {
		std::cout << "Loading file: " << input_path << std::endl;
	} else {
		std::cout << "Path does not exist: " << input_path << std::endl;
		return 1;
	}

	std::ifstream infile(input_path, std::ios::binary);
	if (!infile) {
		std::cout << "Unable to open file: " << input_path << std::endl;
		return 1;
	}

	CompactSimfileHeader csm_header;
	infile.read(reinterpret_cast<char*>(&csm_header), sizeof(CompactSimfileHeader));

	std::cout << "duration: " << csm_header.duration << std::endl;
	std::cout << "time_offset: " << csm_header.time_offset << std::endl;
	std::cout << "event_count: " << csm_header.event_count << std::endl;

	for (uint32_t i = 0; i < csm_header.event_count; i++) {
		SimfileEvent event;
		infile.read(reinterpret_cast<char*>(&event), sizeof(SimfileEvent));
		std::cout << 
			"[" << 
				event.time << ", " << 
				event.duration << ", " <<
				static_cast<int>(event.type) << ", " <<
				static_cast<int>(event.columns) <<  
			"]" << std::endl;
	}

	return 0;
}