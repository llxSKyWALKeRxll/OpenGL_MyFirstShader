#include "Light.h"



Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambient_intensity = 1.0f;

	diffuse_intensity = 0.0f;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight,
			GLfloat red, GLfloat green, GLfloat blue,
			GLfloat a_intensity, GLfloat d_intensity)
{
	shadowMap = new ShadowMap();
	shadowMap->init(shadowWidth, shadowHeight);

	colour = glm::vec3(red, green, blue);
	ambient_intensity = a_intensity;

	diffuse_intensity = d_intensity;
}

Light::~Light()
{
}