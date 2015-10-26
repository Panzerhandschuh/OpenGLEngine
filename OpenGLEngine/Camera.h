#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"

class Camera
{
public:
	GLfloat camSpeed = 8.0f;
	GLfloat yaw, pitch;
	glm::vec3 position;
	glm::mat4 view;

	//Camera();
	void Update(GLfloat deltaTime);
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetForward();
	void LookAt(const glm::vec3& target);
	//glm::mat4 GetViewMatrix();
};

#endif