#include "PointLight.h"


PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
					   GLfloat a_intensity, GLfloat d_intensity, 
					   GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					   GLfloat con, GLfloat lin, GLfloat exp) : Light(1024, 1024, red, green, blue, a_intensity, d_intensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::use_light(GLuint ambient_intensity_location, GLuint ambient_colour_location,
	GLuint diffuse_intensity_location, GLuint position_location,
	GLuint constant_location, GLuint linear_location, GLuint exponent_location)
{
	glUniform3f(ambient_colour_location, colour.x, colour.y, colour.z);
	glUniform1f(ambient_intensity_location, ambient_intensity);

	glUniform1f(diffuse_intensity_location, diffuse_intensity);
	glUniform3f(position_location, position.x, position.y, position.z);
	glUniform1f(constant_location, constant);
	glUniform1f(linear_location, linear);
	glUniform1f(exponent_location, exponent);
}

PointLight::~PointLight()
{
}