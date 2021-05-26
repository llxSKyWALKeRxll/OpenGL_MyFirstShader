#include "Texture.h"



Texture::Texture()
{
	texture_id = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	file_location = "";
}

Texture::Texture(const char* fileLocation)
{
	texture_id = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	file_location = fileLocation;
}

bool Texture::load_texture_jpg()
{
	unsigned char* texture_data = stbi_load(file_location, &width, &height, &bit_depth, 0);
	if (!texture_data)
	{
		printf("Texture location is invalid (for jpg): %s\n", file_location);
		return false;
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texture_data);

	return true;
}

bool Texture::load_texture_png()
{
	unsigned char* texture_data = stbi_load(file_location, &width, &height, &bit_depth, 0);
	if (!texture_data)
	{
		printf("Texture location is invalid (for png): %s\n", file_location);
		return false;
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texture_data);

	return true;
}

void Texture::use_texture()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::clear_texture()
{
	glDeleteTextures(1, &texture_id);
	texture_id = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	file_location = "";
}

Texture::~Texture()
{
	clear_texture();
}