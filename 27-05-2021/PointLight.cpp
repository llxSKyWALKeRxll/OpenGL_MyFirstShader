#include "PointLight.h"


PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
						GLfloat near, GLfloat far,
						GLfloat red, GLfloat green, GLfloat blue, 
					   GLfloat a_intensity, GLfloat d_intensity, 
					   GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					   GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, a_intensity, d_intensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;

	farPlane = far;

	float aspect = (float)shadowWidth / (float)shadowHeight;
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	shadowMap = new OmniShadowMap();
	shadowMap->init(shadowWidth, shadowHeight);
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

std::vector<glm::mat4> PointLight::calculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	//+x, -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	//+y, -y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	//+z, -z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

GLfloat PointLight::getFarPlane()
{
	return farPlane;
}

glm::vec3 PointLight::getPosition()
{
	return position;
}

PointLight::~PointLight()
{
}