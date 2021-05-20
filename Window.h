#pragma once

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();

	Window(GLint window_width, GLint window_height);
	int initialize();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(screen); }
	void swapBuffers() { glfwSwapBuffers(screen); }

	~Window();
private:
	GLFWwindow *screen;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

