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
	bool* getKeys() { return keys; }
	GLfloat getChangeX();
	GLfloat getChangeY();
	void swapBuffers() { glfwSwapBuffers(screen); }

	~Window();
private:
	GLFWwindow *screen;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX, lastY, changeX, changeY;
	bool mouse_first_move;

	void createCallBacks();
	static void handle_keys(GLFWwindow* window, int key, int code, int action, int mode);

	static void handle_mouse(GLFWwindow* window, double xPos, double yPos);
};

