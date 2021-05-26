#include "SpotLight.h"


SpotLight::SpotLight() : PointLight()
{
	direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	edge = 0.0f;
	processedEdge = cosf(glm::radians(edge));
	isOn = true;
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat near, GLfloat far, 
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat a_intensity, GLfloat d_intensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir, 
	GLfloat con, GLfloat lin, GLfloat exp, 
	GLfloat edgyy) : PointLight(shadowWidth, shadowHeight,
								near, far,
								red, green, blue, 
								a_intensity, d_intensity,
								xPos, yPos, zPos,
								con, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = edgyy;
	processedEdge = cosf(glm::radians(edge));
}

void SpotLight::use_light(GLuint ambient_intensity_location, GLuint ambient_colour_location, GLuint diffuse_intensity_location, 
	GLuint position_location, GLuint direction_location, 
	GLuint constant_location, GLuint linear_location, GLuint exponent_location, 
	GLuint edge_location)
{
	glUniform3f(ambient_colour_location, colour.x, colour.y, colour.z);

	if (isOn)
	{
		glUniform1f(ambient_intensity_location, ambient_intensity);
		glUniform1f(diffuse_intensity_location, diffuse_intensity);
	}
	else
	{
		glUniform1f(ambient_intensity_location, 0.0f);
		glUniform1f(diffuse_intensity_location, 0.0f);
	}

	glUniform3f(position_location, position.x, position.y, position.z);
	glUniform1f(constant_location, constant);
	glUniform1f(linear_location, linear);
	glUniform1f(exponent_location, exponent);
	
	glUniform3f(direction_location, direction.x, direction.y, direction.z);
	glUniform1f(edge_location, processedEdge);
}

void SpotLight::set_flash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}