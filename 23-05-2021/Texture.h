#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	void load_texture_jpg();
	void load_texture_png();
	void use_texture();
	void clear_texture();

	~Texture();

private:
	GLuint texture_id;
	int width, height, bit_depth;

	const char* file_location;
};

