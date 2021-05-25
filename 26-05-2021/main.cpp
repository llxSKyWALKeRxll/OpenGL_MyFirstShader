#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <assimp/Importer.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include <vector>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

const float toRadians = 3.141592653589793238f / 180.0f;

GLuint uniform_projection = 0, uniform_model = 0, uniform_view = 0,
uniform_eye_position = 0, uniform_specular_intensity = 0, uniform_shininess = 0,
uniformDirectionalLightTransform = 0;

Window mainWindow;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;
Shader directionalShadowShader;

Camera camera;

Texture t1;
Texture t2;
Texture g1;
Texture everlast;

Material shinyMat;
Material dullMat;

DirectionalLight ambient_light;
PointLight point_lights[MAX_POINT_LIGHTS];
SpotLight spot_lights[MAX_SPOT_LIGHTS];

unsigned int pointLightsCtr = 0;
unsigned int spotLightsCtr = 0;

Model boxingRing1;
Model airPlane1;
Model boxingHeadGuard1;
Model boxingGloves1;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat airplane1Angle = 0.0f;


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
		2, 1, 3
	};

	/*
	0, 2, 1,
	1, 2, 3
	*/

	GLfloat ground_vertices[] = {
		-30.0f, 0.0f, -30.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		30.0f, 0.0f, -30.0f,	30.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-30.0f, 0.0f, 30.0f,	0.0f, 30.0f,	0.0f, -1.0f, 0.0f,
		30.0f, 0.0f, 30.0f,		30.0f, 30.0f,	0.0f, -1.0f, 0.0f
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

	//directionalShadowShader = Shader();
	directionalShadowShader.createFromFiles("Shaders/DirectionalShadowMapVertex.txt", "Shaders/DirectionalShadowMapFragment.txt");
}

void renderScenes()
{
	//TRIANGLE-1
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	t1.use_texture();
	shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
	mesh_list[0]->render_mesh();

	//TRIANGLE-2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	t2.use_texture();
	dullMat.use_material(uniform_specular_intensity, uniform_shininess);
	mesh_list[1]->render_mesh();

	// GROUND
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	g1.use_texture();
	shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
	mesh_list[2]->render_mesh();

	//BOXING RING-1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, -1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.01f, 0.005f, 0.009f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	//t1.use_texture();
	shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
	boxingRing1.renderModel();

	// AIRPLANE-1
	airplane1Angle += 0.1f;
	if (airplane1Angle > 360.0f)
	{
		airplane1Angle = 0.1f;
	}
	model = glm::mat4(1.0f);
	model = glm::rotate(model, -airplane1Angle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(19.2f, 1.5f, 0.0f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -15.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 15.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(camera.getCameraFrontView().x, camera.getCameraFrontView().y, camera.getCameraFrontView().z));
	model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
	//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	//t1.use_texture();
	shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
	airPlane1.renderModel();

	// BOXING HEADGUARD-1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, 0.2f, 1.5f));
	//model = glm::translate(model, glm::vec3(camera.getCameraFrontView().x, camera.getCameraFrontView().y, camera.getCameraFrontView().z));
	model = glm::scale(model, glm::vec3(0.7f, 0.8f, 0.5f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.1f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	//t1.use_texture();
	shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
	boxingHeadGuard1.renderModel();

	// BOXING HEADGUARD-1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.8f, -0.2f, 1.5f));
	//model = glm::translate(model, glm::vec3(camera.getCameraFrontView().x, camera.getCameraFrontView().y, camera.getCameraFrontView().z));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.1f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
	//t1.use_texture();
	shinyMat.use_material(uniform_specular_intensity, uniform_shininess);
	boxingGloves1.renderModel();
}

void directionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.useShader();
	glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());
	
	light->getShadowMap()->write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniform_model = directionalShadowShader.getModelLocation();
	directionalShadowShader.setDirectionalLightTransform(&light->calculateLightTransform());

	renderScenes();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shader_list[0].useShader();
	
	uniform_model = shader_list[0].getModelLocation();
	uniform_projection = shader_list[0].getProjectionLocation();
	uniform_view = shader_list[0].getViewLocation();
	uniform_eye_position = shader_list[0].getEyePosition();
	uniform_specular_intensity = shader_list[0].getSpecularIntensityLocation();
	uniform_shininess = shader_list[0].getShininessLocation();

	glViewport(0, 0, 1280, 1024);

	//Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniform_eye_position, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shader_list[0].setDirectionalLight(&ambient_light);
	shader_list[0].setPointLights(point_lights, pointLightsCtr);
	shader_list[0].setSpotLights(spot_lights, spotLightsCtr);
	shader_list[0].setDirectionalLightTransform(&ambient_light.calculateLightTransform());

	ambient_light.getShadowMap()->read(GL_TEXTURE1);
	shader_list[0].setTexture(0);
	shader_list[0].setDirectionalShadowMap(1);

	glm::vec3 adjust_camera = camera.getCameraPosition();
	adjust_camera.y -= 0.3f;
	spot_lights[0].set_flash(adjust_camera, camera.getCameraDirection());
	
	renderScenes();
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

	everlast = Texture("Textures/Everlast.jpg");
	everlast.load_texture_jpg();


	shinyMat = Material(3.0f, 128);
	dullMat = Material(0.4f, 16);

	boxingRing1 = Model();
	boxingRing1.loadModel("Models/10494_Regulation_Boxing_Ring_SG_V1_Max2011_it2.obj");

	airPlane1 = Model();
	airPlane1.loadModel("Models/11803_Airplane_v1_l1.obj");

	boxingHeadGuard1 = Model();
	boxingHeadGuard1.loadModel("Models/11718_boxing_headgear_v3_L3.obj");

	boxingGloves1 = Model();
	boxingGloves1.loadModel("Models/10493_Leather_Boxing_Gloves_V1_L3.obj");

	ambient_light = DirectionalLight(2048, 2048,
									 1.0f, 1.0f, 1.0f,
									 0.1f, 0.1f,
									 2.0f, -4.0f, -1.0f);

	point_lights[0] = PointLight(0.0f, 1.0f, 0.0f,
								 0.5f, 0.9f,
								 -3.0f, 0.0f, 0.0f,
								 1.0f, 0.2f, 0.1f);
	pointLightsCtr++;

	point_lights[1] = PointLight(1.0f, 0.0f, 0.0f,
                                0.6f, 0.5f,
								3.0f, 0.0f, 0.0f,
								1.0f, 0.3f, 0.2f);
	pointLightsCtr++;

	spot_lights[0] = SpotLight(1.0f, 1.0f, 1.0f,
								0.0f, 2.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f,
								1.0f, 0.0f, 0.0f,
								4.0f);
	spotLightsCtr++;

	spot_lights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightsCtr++;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	Assimp::Importer importer;

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

		directionalShadowMapPass(&ambient_light);
		renderPass(projection, camera.getViewMatrix());

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	return 0;
}