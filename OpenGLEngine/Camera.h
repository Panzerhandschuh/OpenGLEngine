#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::vec3 position;
	glm::mat4 view;

	//Camera();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetForward();
	void LookAt(const glm::vec3& target);
	//glm::mat4 GetViewMatrix();
};

#endif