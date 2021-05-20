#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint window_width, GLint window_height)
{
	width = window_width;
	height = window_height;
}

int Window::initialize()
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
	screen = glfwCreateWindow(width, height, "Sky", NULL, NULL);
	if (!screen)
	{
		printf("GLFW window setup failed! :(");
		glfwTerminate();
		return 1;
	}

	//Buffer size information
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

	glEnable(GL_DEPTH_TEST);

	//Setup ViewPort size
	glViewport(0, 0, bufferHeight, bufferWidth);
}

Window::~Window()
{
	glfwDestroyWindow(screen);
	glfwTerminate();
}