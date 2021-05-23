#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:

	Camera();

	Camera(glm::vec3 sPosition, glm::vec3 sUp, GLfloat sYaw, GLfloat sPitch, GLfloat sMoveSpeed, GLfloat sTurnSpeed);

	void key_control(bool* keys, GLfloat deltaTime);

	void mouse_control(GLfloat changeX, GLfloat changeY);

	glm::vec3 getCameraPosition();

	glm::mat4 getViewMatrix();

	~Camera();
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

