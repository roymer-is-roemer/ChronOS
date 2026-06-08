#include "image.hpp"
#include <OpenGL/gl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

Image::Image(std::string filename) {
	FILE* fp = std::fopen(filename.c_str(), "rb");

	if (!fp) {
        std::cerr << "Error: Could not open image file: " << filename << std::endl;
		return;
    }

	uint16_t width, height;
	fread(&width, sizeof(width), 1, fp);
	fread(&height, sizeof(height), 1, fp);
	
	this->bitmap = (unsigned char*)malloc(width * height * 3);
	fread(this->bitmap, height * width * 3, 1, fp);
	fclose(fp);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &this->gl_texture_id);
	glBindTexture(GL_TEXTURE_2D, this->gl_texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->bitmap);
}

Image::~Image() {
	glDeleteTextures(1, &this->gl_texture_id);
	free(this->bitmap);
}
