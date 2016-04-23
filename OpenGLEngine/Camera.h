#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"
#include "Engine.h"
#include "Component.h"
#include "Transform.h"

class Camera : public Component
{
public:
	static Camera* main;

	GLfloat sensitivity = 0.125f;
	GLfloat speed = 15.0f;
	GLfloat fastSpeed = 45.0f;
	glm::mat4 view;
	glm::mat4 proj;

	void Start();
	void Update(GLfloat deltaTime);
	//void LookAt(const glm::vec3& target);
	glm::mat4 GetView();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetForward();

private:
	GLfloat yaw, pitch;
};

#endif