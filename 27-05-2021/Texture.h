#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	bool load_texture_jpg();
	bool load_texture_png();

	void use_texture();
	void clear_texture();

	~Texture();

private:
	GLuint texture_id;
	int width, height, bit_depth;

	const char* file_location;
};

