#include <string>
#include <iostream>
#include <fstream>

#include "ppm.hpp"

//#define DEBUG

const char kPpmMagic[] = {0x50, 0x36, 0x00}; // "P6"

/*
 * Seek the file stream to the beginning of the next available data.
 * Skips comments and whitespace.
 */
static void to_next_data(std::ifstream& file) {
	char input;
	while (true) {
		file.get(input);

#ifdef DEBUG
		std::cout << "Position: " << file.tellg() << ", input: \"" << input << "\"" << std::endl;
#endif

		if (input == '#') {
			// This format is so retarded that it allows comments IN A BINARY FILE, awesome.
			while (file.get(input)) {
				if (input == '\n') {
					break;
				}
			}
		} else if (isspace(input)) {
			continue;
		} else {
			// We got data, let's stop right there
			break;
		}
	}

	// Get us back to before the parsed char in the loop
	file.seekg(-1, std::ios::cur);
}

/*
 * Get a string from the file stream.
 * Will automatically skip comments and whitespace.
 */
static std::string get_line(std::ifstream& file) {
	std::string line;

	to_next_data(file);

	char input;
	while (true) {
		file.get(input);

#ifdef DEBUG
		std::cout << "Position: " << file.tellg() << ", input: \"" << input << "\"" << std::endl;
#endif

		if (isspace(input)) {
			break;
		} else {
			line += input;
		}
	}

	return line;
}

ppm read_ppm(const std::string& filename) {
	ppm ppm_data;
	std::ifstream ppm_file(filename, std::ios::in | std::ios::binary);
	if (!ppm_file.is_open()) {
		std::cout << "Could not open file: " << filename << std::endl;
		return ppm_data;
	}

	std::string magic_number = get_line(ppm_file);
	if (magic_number != kPpmMagic) {
		std::cout << "Invalid magic number: " << magic_number << std::endl;
		return ppm_data;
	}

	ppm_data.width = stoi(get_line(ppm_file));
	ppm_data.height = stoi(get_line(ppm_file));
	ppm_data.max_color = stoi(get_line(ppm_file));

	to_next_data(ppm_file);

	ppm_data.pixel_len = ppm_data.width * ppm_data.height;

	// Allocate memory for the pixels
	ppm_data.pixels = new pixel[ppm_data.pixel_len];
	for (unsigned int i = 0; i < ppm_data.pixel_len; i++) {
		ppm_file.read((char*)&ppm_data.pixels[i].r, 1);
		ppm_file.read((char*)&ppm_data.pixels[i].g, 1);
		ppm_file.read((char*)&ppm_data.pixels[i].b, 1);
	}

	return ppm_data;
}

/*
int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: ppm <filename>" << std::endl;
		return 1;
	}
	std::string filename(argv[1]);

	ppm ppm_data = read_ppm(filename);

	std::cout << "Width: " << ppm_data.width << std::endl;
	std::cout << "Height: " << ppm_data.height << std::endl;
	std::cout << "Max color: " << ppm_data.max_color << std::endl;

	return 0;
}
*/
