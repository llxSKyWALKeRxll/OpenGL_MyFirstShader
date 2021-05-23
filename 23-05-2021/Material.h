#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void use_material(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();
	
private:
	GLfloat specular_intensity;
	GLfloat shininess;
};

