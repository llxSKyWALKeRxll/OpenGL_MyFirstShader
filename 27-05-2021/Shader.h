#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\glm.hpp>

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
	void createFromFiles(const char* vLocation, const char* gLocation,const char* fLocation);
	std::string readFile(const char* file_location);

	void validate();

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
	GLuint getOmniLightPosLocation();
	GLuint getFarPlaneLocation();

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLight, unsigned int pCount, unsigned int textureUnit, unsigned int offset);
	void setSpotLights(SpotLight* sLight, unsigned int sCount, unsigned int textureUnit, unsigned int offset);
	void setTexture(GLuint textureUnit);
	void setDirectionalShadowMap(GLuint textureUnit);
	void setDirectionalLightTransform(glm::mat4* DLTransform);
	void setLightMatrices(std::vector<glm::mat4> lightMatrices);

	void useShader();
	void clearShader();

	~Shader();

private:

	int pointLightCounter;
	int spotLightCounter;

	GLuint shader_id, uniform_projection, uniform_model, uniform_view, uniform_eye_position, 
		   uniform_specular_intensity, uniform_shininess,
	       uniformTexture,
		   uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformFarPlane;

	GLuint uniformLightMatrices[6];

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

	struct
	{
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void compileShader(const char* vCode, const char* fCode);
	void compileShader(const char* vCode, const char* gCode, const char* fCode);
	void addShader(GLuint main_program, const char* shader_code, GLenum shader_type);

	void compileProgram();
};

