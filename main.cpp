#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>

//Windows dimensions
const GLint width = 800, height = 600;

GLuint VAO, VBO, shader;

//Vertex Shader
static const char* vShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
layout (location = 0) in vec3 pos;                                  \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);           \n\
}";

//Fragment Shader
static const char* fShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
out vec4 colour;                                                    \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
colour = vec4(0.0f, 0.6f, 0.0f, 1.0);                               \n\
}";

void create_triangle()
{
	//Vertices for the triangle
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		2.5f, -3.5f, 0.0f,
		0.3f, 0.5f, 0.0f
	};

	//Binding the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Binding the Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Unbinding VAO AND VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void add_shader(GLuint main_program, const char* shader_code, GLenum shader_type)
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

void compile_shaders()
{
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Shader setup failed! :(\n");
		return;
	}

	add_shader(shader, vShader, GL_VERTEX_SHADER);
	add_shader(shader, fShader, GL_FRAGMENT_SHADER);

	//For debugging the code.
	//Intellisense cannot tell if there are any errors in the shaders code.
	//Hence, this is worth the effort!
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error Linking Program:-\n'%s'", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error Validating Program:-\n'%s'", eLog);
		return;
	}
}

int main()
{
	if (!glfwInit())
	{
		printf("GLFW Initialization failed! :(");
		glfwTerminate();
		return 1;
	}

	//SETTING UP GLFW WINDOW PROPERTIES
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //3.3

	//Core profile = No backwards compatability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Allow forward compatibility

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//SETTING UP DISPLAY WINDOW
	GLFWwindow *screen = glfwCreateWindow(width, height, "Sky", NULL, NULL);
	if (!screen)
	{
		printf("GLFW window setup failed! :(");
		glfwTerminate();
		return 1;
	}

	//Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(screen, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(screen);

	//Allow modern extension features
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed! :(");
		glfwDestroyWindow(screen);
		glfwTerminate();
		return 1;
	}

	//Setup ViewPort size
	glViewport(0, 0, bufferHeight, bufferWidth);

	create_triangle();
	compile_shaders();

	//Main loop
	while (!glfwWindowShouldClose(screen))
	{
		//Get and handle user input events (clicking a button, pressing a button, etc.)
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(screen);
	}
	return 0;
}