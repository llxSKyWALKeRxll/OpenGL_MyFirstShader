#pragma once

#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
		             GLfloat a_intensity, GLfloat d_intensity,
		             GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void use_light(GLfloat ambient_intensity_location, GLfloat ambient_colour_location,
				   GLfloat diffuse_intensity_location, GLfloat direction_location);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

