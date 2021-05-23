#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include <vector>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.141592653589793238f / 180.0f;

Window mainWindow;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;

Camera camera;

Texture t1;
Texture t2;
Texture g1;

Material shinyMat;
Material dullMat;

DirectionalLight ambient_light;
PointLight point_lights[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


//Vertex Shader
static const char* vShader = "Shaders/VertexShader.txt";

//Fragment Shader
static const char* fShader = "Shaders/FragmentShader.txt";

void calcAvgNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	                unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2); 
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

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
		-1.0f, -1.0f, -0.7f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.7f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int ground_indices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat ground_vertices[] = {
		-15.0f, 0.0f, -15.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, -15.0f,	15.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-15.0f, 0.0f, 15.0f,	0.0f, 15.0f,	0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, 15.0f,		15.0f, 15.0f,	0.0f, -1.0f, 0.0f
	};

	calcAvgNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->create_mesh(vertices, indices, 32, 12);
	mesh_list.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->create_mesh(vertices, indices, 32, 12);
	mesh_list.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->create_mesh(ground_vertices, ground_indices, 32, 6);
	mesh_list.push_back(obj3);
}


void create_shaders()
{
	Shader* shader1 = new Shader();
	shader1->createFromFiles(vShader, fShader);
	shader_list.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1280, 1024);
	mainWindow.initialize();

	create_objects();
	create_shaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 4.0f, 0.4f);

	t1 = Texture("Textures/t5.png");
	t1.load_texture_jpg();

	t2 = Texture("Textures/t2.png");
	t2.load_texture_jpg();

	g1 = Texture("Textures/g1.png");
	g1.load_texture_png();

	shinyMat = Material(3.0f, 128);
	dullMat = Material(0.4f, 16);

	ambient_light = DirectionalLight(1.0f, 1.0f, 1.0f,
									 0.0f, 0.3f,
									 2.0f, -1.0f, -2.0f);

	unsigned int pointLightsCtr = 0;
	point_lights[0] = PointLight(0.0f, 1.0f, 0.0f,
								 0.0f, 1.0f,
								 -3.0f, 0.0f, 0.0f,
								 1.0f, 0.2f, 0.1f);
	pointLightsCtr++;

	point_lights[1] = PointLight(1.0f, 0.0f, 0.0f,
                                0.0f, 1.0f,
								3.0f, 0.0f, 0.0f,
								1.0f, 0.3f, 0.2f);
	pointLightsCtr++;

	GLuint uniform_projection = 0, uniform_model = 0, uniform_view = 0,
		   uniform_eye_position = 0, uniform_specular_intensity = 0, uniform_shininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Main loop
	while (!mainWindow.getShouldClose())
	{

		GLfloat currentTime = glfwGetTime(); //FOR SDL --> SDL_GetPerformanceCounter();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Get and handle user input events (clicking a button, pressing a button, etc.)
		glfwPollEvents();

		camera.key_control(mainWindow.getKeys(), deltaTime);
		camera.mouse_control(mainWindow.getChangeX(), mainWindow.getChangeY());

		//Clear window
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_list[0].useShader();
		uniform_model = shader_list[0].getModelLocation();
		uniform_projection = shader_list[0].getProjectionLocation();
		uniform_view = shader_list[0].getViewLocation();
		uniform_eye_position = shader_list[0].getEyePosition();
		uniform_specular_intensity = shader_list[0].getSpecularIntensityLocation();
		uniform_shininess = shader_list[0].getShininessLocation();

		shader_list[0].setDirectionalLight(&ambient_light);

		shader_list[0].setPointLights(point_lights, pointLightsCtr);




		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		glUniform3f(uniform_eye_position, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f);
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(triangle_offset, 0.0f, 0.0f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		//glUniform1f(uniform_model, triangle_offset);
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		t1.use_texture();
		shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
		mesh_list[0]->render_mesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		t2.use_texture();
		dullMat.use_material(uniform_specular_intensity, uniform_shininess);
		mesh_list[1]->render_mesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		g1.use_texture();
		shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
		mesh_list[2]->render_mesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	return 0;
}