#include <string>

/*
 * Each pixel is a triplet of red, green, and blue samples, in that order.
 * Each sample is represented in pure binary by either 1 or 2 bytes.
 * If the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes.
 * 
 * We're gonna ignore the 2 bytes usecase for now.
 */
struct pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

// Reference: http://netpbm.sourceforge.net/doc/ppm.html
struct ppm {
	unsigned int width;
	unsigned int height;
	unsigned int max_color;

	pixel *pixels; // pixels array (size is width * height)
	size_t pixel_len;
};

ppm read_ppm(const std::string& filename);
