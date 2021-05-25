#include "Shader.h"

Shader::Shader()
{
	shader_id = 0;
	uniform_model = 0;
	uniform_projection = 0;
	pointLightCounter = 0;
	spotLightCounter = 0;
}

void Shader::createFromString(const char* vCode, const char* fCode)
{
	compileShader(vCode, fCode);
}

void Shader::createFromFiles(const char* vLocation, const char* fLocation)
{
	std::string vString = readFile(vLocation);
	std::string fString = readFile(fLocation);
	const char* vCode = vString.c_str();
	const char* fCode = fString.c_str();

	compileShader(vCode, fCode);
}

std::string Shader::readFile(const char* file_location)
{
	std::string content;
	std::ifstream fileStream(file_location, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File does not exist.", file_location);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(const char* vCode, const char* fCode)
{
	shader_id = glCreateProgram();
	if (!shader_id)
	{
		printf("Shader setup failed! :(\n");
		return;
	}

	addShader(shader_id, vCode, GL_VERTEX_SHADER);
	addShader(shader_id, fCode, GL_FRAGMENT_SHADER);

	//For debugging the code.
	//Intellisense cannot tell if there are any errors in the shaders code.
	//Hence, this is worth the effort!
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader_id);
	glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
		printf("Error Linking Program:- '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader_id);
	glGetProgramiv(shader_id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
		printf("Error Validating Program:- '%s'\n", eLog);
		return;
	}

	uniform_model = glGetUniformLocation(shader_id, "model");
	uniform_projection = glGetUniformLocation(shader_id, "projection");
	uniform_view = glGetUniformLocation(shader_id, "view");
	uniformDirectionalLight.uniform_colour = glGetUniformLocation(shader_id, "directional_light.base.colour");
	uniformDirectionalLight.uniform_ambient_intensity = glGetUniformLocation(shader_id, "directional_light.base.ambient_intensity");
	uniformDirectionalLight.uniform_direction = glGetUniformLocation(shader_id, "directional_light.direction");
	uniformDirectionalLight.uniform_diffuse_intensity = glGetUniformLocation(shader_id, "directional_light.base.diffuse_intensity");
	uniform_specular_intensity = glGetUniformLocation(shader_id, "material.specular_intensity");
	uniform_shininess = glGetUniformLocation(shader_id, "material.shininess");
	uniform_eye_position = glGetUniformLocation(shader_id, "eyePosition");

	uniformPointLightCounter = glGetUniformLocation(shader_id, "pointLightCounter");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].base.colour", i);
		uniformPointLight[i].uniform_colour = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].base.ambient_intensity", i);
		uniformPointLight[i].uniform_ambient_intensity = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].base.diffuse_intensity", i);
		uniformPointLight[i].uniform_diffuse_intensity = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].position", i);
		uniformPointLight[i].uniform_position = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].constant", i);
		uniformPointLight[i].uniform_constant = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].linear", i);
		uniformPointLight[i].uniform_linear = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].exponent", i);
		uniformPointLight[i].uniform_exponent = glGetUniformLocation(shader_id, locBuff);
	}

	uniformSpotLightCounter = glGetUniformLocation(shader_id, "spotLightCounter");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].base.base.colour", i);
		uniformSpotLight[i].uniform_colour = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "point_lights[%d].base.base.ambient_intensity", i);
		uniformSpotLight[i].uniform_ambient_intensity = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].base.base.diffuse_intensity", i);
		uniformSpotLight[i].uniform_diffuse_intensity = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].base.position", i);
		uniformSpotLight[i].uniform_position = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].base.constant", i);
		uniformSpotLight[i].uniform_constant = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].base.linear", i);
		uniformSpotLight[i].uniform_linear = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].base.exponent", i);
		uniformSpotLight[i].uniform_exponent = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].direction", i);
		uniformSpotLight[i].uniform_direction = glGetUniformLocation(shader_id, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spot_lights[%d].edge", i);
		uniformSpotLight[i].uniform_edge = glGetUniformLocation(shader_id, locBuff);
	}

	uniformTexture = glGetUniformLocation(shader_id, "the_texture");
	uniformDirectionalLightTransform = glGetUniformLocation(shader_id, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shader_id, "directionalShadowMap");
}

void Shader::setDirectionalLight(DirectionalLight* dLight)
{
	dLight->use_light(uniformDirectionalLight.uniform_ambient_intensity, uniformDirectionalLight.uniform_colour,
					  uniformDirectionalLight.uniform_diffuse_intensity, uniformDirectionalLight.uniform_direction);
}

void Shader::setPointLights(PointLight* pLight, unsigned int pCount)
{
	if (pCount > MAX_POINT_LIGHTS) pCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCounter, pCount);

	for (size_t i = 0; i < pCount; i++)
	{
		pLight[i].use_light(uniformPointLight[i].uniform_ambient_intensity, uniformPointLight[i].uniform_colour,
			uniformPointLight[i].uniform_diffuse_intensity, uniformPointLight[i].uniform_position,
			uniformPointLight[i].uniform_constant, uniformPointLight[i].uniform_linear, uniformPointLight[i].uniform_exponent);
	}
	/*
	* pLight[i].use_light(uniformPointLight[i].uniform_ambient_intensity, uniformPointLight[i].uniform_colour
			, uniformPointLight[i].uniform_diffuse_intensity, uniformPointLight[i].uniform_position
			, uniformPointLight[i].uniform_constant, uniformPointLight[i].uniform_linear, uniformPointLight[i].uniform_exponent);
	*/
}

void Shader::setSpotLights(SpotLight* sLight, unsigned int sCount)
{
	if (sCount > MAX_SPOT_LIGHTS) sCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCounter, sCount);

	for (size_t i = 0; i < sCount; i++)
	{
		sLight[i].use_light(uniformSpotLight[i].uniform_ambient_intensity, uniformSpotLight[i].uniform_colour,
			uniformSpotLight[i].uniform_diffuse_intensity, uniformSpotLight[i].uniform_position, uniformSpotLight[i].uniform_direction,
			uniformSpotLight[i].uniform_constant, uniformSpotLight[i].uniform_linear, uniformSpotLight[i].uniform_exponent,
			uniformSpotLight[i].uniform_edge);
	}
}

GLuint Shader::getProjectionLocation()
{
	return uniform_projection;
}

GLuint Shader::getModelLocation()
{
	return uniform_model;
}

GLuint Shader::getViewLocation()
{
	return uniform_view;
}

GLuint Shader::getAmbientColourLocation()
{
	return uniformDirectionalLight.uniform_colour;
}

GLuint Shader::getAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniform_ambient_intensity;
}

GLuint Shader::getDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniform_diffuse_intensity;
}

GLuint Shader::getDirectionLocation()
{
	return uniformDirectionalLight.uniform_direction;
}

GLuint Shader::getSpecularIntensityLocation()
{
	return uniform_specular_intensity;
}

GLuint Shader::getShininessLocation()
{
	return uniform_shininess;
}

GLuint Shader::getEyePosition()
{
	return uniform_eye_position;
}

void Shader::useShader()
{
	glUseProgram(shader_id);
}

void Shader::clearShader()
{
	if (shader_id != 0)
	{
		glDeleteProgram(shader_id);
		shader_id = 0;
	}

	uniform_model = 0;
	uniform_projection = 0;
}

void Shader::addShader(GLuint main_program, const char* shader_code, GLenum shader_type)
{
	GLuint the_shader = glCreateShader(shader_type);

	const GLchar* the_code[1];
	the_code[0] = shader_code;

	GLint code_length[1];
	code_length[0] = strlen(shader_code);

	glShaderSource(the_shader, 1, the_code, code_length);
	glCompileShader(the_shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(the_shader, sizeof(eLog), NULL, eLog);
		printf("Error Compiling %d shader:- '%s'\n", shader_type, eLog);
		return;
	}

	glAttachShader(main_program, the_shader);
}

void Shader::setTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4* DLTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*DLTransform));
}

Shader::~Shader()
{
	clearShader();
}