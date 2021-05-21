#include "Camera.h"

Camera::Camera()
{
}


Camera::Camera(glm::vec3 sPosition, glm::vec3 sUp, GLfloat sYaw, GLfloat sPitch, GLfloat sMoveSpeed, GLfloat sTurnSpeed)
{
	position = sPosition;
	worldUp = sUp;
	yaw = sYaw;
	pitch = sPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	moveSpeed = sMoveSpeed;
	turnSpeed = sTurnSpeed;

	update();
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::key_control(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (keys[GLFW_KEY_SPACE])
	{
		position += worldUp * velocity;
	}
	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		position -= worldUp * velocity;
	}
}

void Camera::mouse_control(GLfloat changeX, GLfloat changeY)
{
	changeX *= turnSpeed;
	changeY *= turnSpeed;

	yaw += changeX;
	pitch += changeY;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

Camera::~Camera()
{
}