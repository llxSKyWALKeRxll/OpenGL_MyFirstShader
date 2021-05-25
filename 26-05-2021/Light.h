#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm\gtc\matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight,
			GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat a_intensity, GLfloat d_intensity);

	ShadowMap* getShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambient_intensity;
	GLfloat diffuse_intensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

