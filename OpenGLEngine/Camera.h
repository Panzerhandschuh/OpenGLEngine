#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"
#include "Engine.h"

class Camera
{
public:
	static Camera main;

	GLfloat sensitivity = 0.125f;
	GLfloat speed = 15.0f;
	GLfloat fastSpeed = 45.0f;
	glm::vec3 position;
	glm::mat4 view;
	glm::mat4 proj;

	Camera();
	void Update(GLfloat deltaTime);
	void LookAt(const glm::vec3& target);
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetForward();

private:
	GLfloat yaw, pitch;
};

#endif