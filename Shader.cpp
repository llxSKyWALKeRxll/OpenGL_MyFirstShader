#include "Shader.h"

Shader::Shader()
{
	shader_id = 0;
	uniform_model = 0;
	uniform_projection = 0;
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
		printf("Error Linking Program:-\n'%s'", eLog);
		return;
	}

	glValidateProgram(shader_id);
	glGetProgramiv(shader_id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
		printf("Error Validating Program:-\n'%s'", eLog);
		return;
	}

	uniform_model = glGetUniformLocation(shader_id, "model");
	uniform_projection = glGetUniformLocation(shader_id, "projection");
}

GLuint Shader::getProjectionLocation()
{
	return uniform_projection;
}

GLuint Shader::getModelLocation()
{
	return uniform_model;
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
		glGetProgramInfoLog(the_shader, sizeof(eLog), NULL, eLog);
		printf("Error Compiling %d shader:-\n'%s'", shader_type, eLog);
		return;
	}

	glAttachShader(main_program, the_shader);
}

Shader::~Shader()
{
	clearShader();
}