#include "Material.h"


Material::Material()
{
	specular_intensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specular_intensity = sIntensity;
	shininess = shine;
}

void Material::use_material(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specular_intensity);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}