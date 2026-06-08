#include <string>
#include <cstdint>

class Image {
public:
	uint16_t width;
	uint16_t height;
	unsigned char* bitmap;
	unsigned int gl_texture_id;

	Image(std::string filename);
	~Image();
};
