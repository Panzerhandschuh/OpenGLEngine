#include "Camera.h"

Camera Camera::main;

Camera::Camera()
{
	proj = glm::perspective(glm::quarter_pi<GLfloat>(),
		(GLfloat)Engine::screenWidth / (GLfloat)Engine::screenHeight, 0.1f, 1000.0f);
}

void Camera::Update(GLfloat deltaTime)
{
	// Move camera
	if (InputManager::GetKey(GLFW_KEY_W))
		position += speed * -GetForward() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_S))
		position += speed * GetForward() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_A))
		position += speed * -GetRight() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_D))
		position += speed * GetRight() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_Q))
		position += speed * -GetUp() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_E))
		position += speed * GetUp() * deltaTime;

	// Mouse look
	if (InputManager::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec2 mouseDelta = InputManager::mouseDelta;
		yaw += mouseDelta.x * sensitivity;
		pitch += mouseDelta.y * sensitivity;
	}

	// Clip the pitch range of the camera
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 up = glm::normalize(glm::cross(right, front));

	//camera.view = glm::mat4(right.x, right.y, right.z, 0.0f, 
	//	up.x, up.y, up.z, 0.0f, 
	//	front.x, front.y, front.z, 0.0f, 
	//	0.0f, 0.0f, 0.0f, 1.0f);
	view = glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::GetRight()
{
	return glm::vec3(view[0][0], view[1][0], view[2][0]);
}

glm::vec3 Camera::GetUp()
{
	return glm::vec3(view[0][1], view[1][1], view[2][1]);
}

glm::vec3 Camera::GetForward()
{
	return glm::vec3(view[0][2], view[1][2], view[2][2]);
}

void Camera::LookAt(const glm::vec3& target)
{
	view = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}