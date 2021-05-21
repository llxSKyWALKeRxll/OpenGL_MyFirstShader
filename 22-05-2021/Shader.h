#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <fstream>

class Shader
{
public:
	Shader();

	void createFromString(const char* vCode, const char* fCode);
	void createFromFiles(const char* vLocation, const char* fLocation);
	std::string readFile(const char* file_location);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();

	void useShader();
	void clearShader();

	~Shader();

private:
	GLuint shader_id, uniform_projection, uniform_model, uniform_view;

	void compileShader(const char* vCode, const char* fCode);
	void addShader(GLuint main_program, const char* shader_code, GLenum shader_type);
};

