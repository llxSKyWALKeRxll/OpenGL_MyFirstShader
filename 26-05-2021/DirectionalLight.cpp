#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat a_intensity, GLfloat d_intensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight,
		red, green, blue,
		a_intensity, d_intensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 100.0f);
}

void DirectionalLight::use_light(GLfloat ambient_intensity_location, GLfloat ambient_colour_location,
	GLfloat diffuse_intensity_location, GLfloat direction_location)
{
	glUniform3f(ambient_colour_location, colour.x, colour.y, colour.z);
	glUniform1f(ambient_intensity_location, ambient_intensity);

	glUniform3f(direction_location, direction.x, direction.y, direction.z);
	glUniform1f(diffuse_intensity_location, diffuse_intensity);
}

glm::mat4 DirectionalLight::calculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}