#include "Camera.h"

//Camera::Camera()
//{
//	position = glm::vec3(0.0f, 0.0f, 5.0f);
//}

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