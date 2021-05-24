#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <fstream>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


class Shader
{
public:
	Shader();

	void createFromString(const char* vCode, const char* fCode);
	void createFromFiles(const char* vLocation, const char* fLocation);
	std::string readFile(const char* file_location);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getEyePosition();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLight, unsigned int pCount);
	void setSpotLights(SpotLight* sLight, unsigned int sCount);


	void useShader();
	void clearShader();

	~Shader();

private:

	int pointLightCounter;
	int spotLightCounter;

	GLuint shader_id, uniform_projection, uniform_model, uniform_view, uniform_eye_position, 
		   uniform_specular_intensity, uniform_shininess;

	struct {
		GLuint uniform_colour;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;
		GLuint uniform_direction;
	} uniformDirectionalLight;

	GLuint uniformPointLightCounter;

	struct {
		GLuint uniform_colour;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;
		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCounter;

	struct
	{
		GLuint uniform_colour;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;
		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;
		GLuint uniform_direction;
		GLuint uniform_edge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	void compileShader(const char* vCode, const char* fCode);
	void addShader(GLuint main_program, const char* shader_code, GLenum shader_type);
};

