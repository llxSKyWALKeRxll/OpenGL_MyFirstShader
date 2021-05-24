#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity, GLfloat d_intensity);

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambient_intensity;

	GLfloat diffuse_intensity;
};

