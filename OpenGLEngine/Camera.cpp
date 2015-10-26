#include "Camera.h"

//Camera::Camera()
//{
//	position = glm::vec3(0.0f, 0.0f, 5.0f);
//}

void Camera::Update(GLfloat deltaTime)
{
	// Move camera
	if (InputManager::keys[GLFW_KEY_W])
		position += camSpeed * -GetForward() * deltaTime;
	if (InputManager::keys[GLFW_KEY_S])
		position += camSpeed * GetForward() * deltaTime;
	if (InputManager::keys[GLFW_KEY_A])
		position += camSpeed * -GetRight() * deltaTime;
	if (InputManager::keys[GLFW_KEY_D])
		position += camSpeed * GetRight() * deltaTime;
	if (InputManager::keys[GLFW_KEY_Q])
		position += camSpeed * -GetUp() * deltaTime;
	if (InputManager::keys[GLFW_KEY_E])
		position += camSpeed * GetUp() * deltaTime;

	// Mouse look
	glm::vec2 mouseDelta = InputManager::mouseDelta;
	yaw += mouseDelta.x;
	pitch += mouseDelta.y;

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