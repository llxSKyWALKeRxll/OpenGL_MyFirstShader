#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
								   GLfloat a_intensity, GLfloat d_intensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, a_intensity, d_intensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::use_light(GLfloat ambient_intensity_location, GLfloat ambient_colour_location,
	GLfloat diffuse_intensity_location, GLfloat direction_location)
{
	glUniform3f(ambient_colour_location, colour.x, colour.y, colour.z);
	glUniform1f(ambient_intensity_location, ambient_intensity);

	glUniform3f(direction_location, direction.x, direction.y, direction.z);
	glUniform1f(diffuse_intensity_location, diffuse_intensity);
}

DirectionalLight::~DirectionalLight()
{
}