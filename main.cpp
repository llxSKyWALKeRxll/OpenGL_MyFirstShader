#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Mesh.h"
#include <vector>
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.141592653589793238f / 180.0f;

Window mainWindow;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;



//Vertex Shader
static const char* vShader = "Shaders/VertexShader.txt";

//Fragment Shader
static const char* fShader = "Shaders/FragmentShader.txt";

void create_objects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	//Vertices for the triangle
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->create_mesh(vertices, indices, 12, 12);
	mesh_list.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->create_mesh(vertices, indices, 12, 12);
	mesh_list.push_back(obj2);
}


void create_shaders()
{
	Shader* shader1 = new Shader();
	shader1->createFromFiles(vShader, fShader);
	shader_list.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.initialize();

	create_objects();
	create_shaders();

	GLuint uniform_projection = 0, uniform_model = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Main loop
	while (!mainWindow.getShouldClose())
	{
		//Get and handle user input events (clicking a button, pressing a button, etc.)
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_list[0].useShader();
		uniform_model = shader_list[0].getModelLocation();
		uniform_projection = shader_list[0].getProjectionLocation();

		glm::mat4 model(1.0f);
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(triangle_offset, 0.0f, 0.0f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.4f, -3.0f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//glUniform1f(uniform_model, triangle_offset);
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));

		mesh_list[0]->render_mesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));

		mesh_list[1]->render_mesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	return 0;
}