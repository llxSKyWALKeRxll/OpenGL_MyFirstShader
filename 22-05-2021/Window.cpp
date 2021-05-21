#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	changeX = 0.0f;
	changeY = 0.0f;
}

Window::Window(GLint window_width, GLint window_height)
{
	width = window_width;
	height = window_height;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	changeX = 0.0f;
	changeY = 0.0f;
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

	//Handling input for keyboard and mouse
	createCallBacks();

	glfwSetInputMode(screen, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glfwSetWindowUserPointer(screen, this);
}

void Window::createCallBacks()
{
	glfwSetKeyCallback(screen, handle_keys);
	glfwSetCursorPosCallback(screen, handle_mouse);
}

GLfloat Window::getChangeX()
{
	GLfloat temp_changeX = changeX;
	changeX = 0.0f;
	return temp_changeX;
}

GLfloat Window::getChangeY()
{
	GLfloat temp_changeY = changeY;
	changeY = 0.0f;
	return temp_changeY;
}

void Window::handle_keys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* temp_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			temp_window->keys[key] = true;
			//printf("%d key pressed!\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			temp_window->keys[key] = false;
			//printf("%d key released!\n", key);
		}
	}
}

void Window::handle_mouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* temp_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (temp_window->mouse_first_move)
	{
		temp_window->lastX = xPos;
		temp_window->lastY = yPos;
		temp_window->mouse_first_move = false;
	}

	temp_window->changeX = xPos - temp_window->lastX;
	temp_window->changeY = temp_window->lastY - yPos;

	temp_window->lastX = xPos;
	temp_window->lastY = yPos;

	//printf("x: %.6f, y: %.6f\n", temp_window->changeX, temp_window->changeY);
}

Window::~Window()
{
	glfwDestroyWindow(screen);
	glfwTerminate();
}