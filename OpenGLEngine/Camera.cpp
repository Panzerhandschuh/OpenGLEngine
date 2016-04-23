#include "Camera.h"

using namespace glm;

Camera* Camera::main;

void Camera::Start()
{
	proj = perspective(quarter_pi<GLfloat>(),
		(GLfloat)Engine::screenWidth / (GLfloat)Engine::screenHeight, 0.1f, 1000.0f);
}

void Camera::Update(GLfloat deltaTime)
{
	// Move camera
	GLfloat camSpeed = (InputManager::GetKey(GLFW_KEY_LEFT_SHIFT)) ? fastSpeed : speed;
	if (InputManager::GetKey(GLFW_KEY_W))
		transform->position += camSpeed * -GetForward() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_S))
		transform->position += camSpeed * GetForward() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_A))
		transform->position += camSpeed * -GetRight() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_D))
		transform->position += camSpeed * GetRight() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_Q))
		transform->position += camSpeed * -GetUp() * deltaTime;
	if (InputManager::GetKey(GLFW_KEY_E))
		transform->position += camSpeed * GetUp() * deltaTime;

	// Mouse look
	if (InputManager::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	{
		vec2 mouseDelta = InputManager::mouseDelta;
		yaw += mouseDelta.x * sensitivity;
		pitch += mouseDelta.y * sensitivity;
	}

	// Clip the pitch range of the camera
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 front;
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = normalize(front);
	vec3 right = normalize(cross(front, vec3(0.0f, 1.0f, 0.0f)));
	vec3 up = normalize(cross(right, front));

	//view = glm::mat4(right.x, right.y, right.z, 0.0f, 
	//	up.x, up.y, up.z, 0.0f, 
	//	front.x, front.y, front.z, 0.0f, 
	//	0.0f, 0.0f, 0.0f, 1.0f);
	view = glm::lookAt(transform->position, transform->position + front, up);
	
	//transform->rotation = quat(lookAt(transform->position, transform->position + front, up));
	//transform->rotation = quat_cast(view);
}

vec3 Camera::GetRight()
{
	return vec3(view[0][0], view[1][0], view[2][0]);
}

vec3 Camera::GetUp()
{
	return vec3(view[0][1], view[1][1], view[2][1]);
}

vec3 Camera::GetForward()
{
	return vec3(view[0][2], view[1][2], view[2][2]);
}

//void Camera::LookAt(const glm::vec3& target)
//{
//	view = lookAt(transform->position, target, glm::vec3(0.0f, 1.0f, 0.0f));
//}

mat4 Camera::GetView()
{
	//return lookAt(transform->position, transform->position + transform->GetForward(), transform->GetUp());

	//mat4 view(transform->rotation);
	//
	//vec3 xAxis(view[0][0], view[1][0], view[2][0]);
	//vec3 yAxis(view[0][1], view[1][1], view[2][1]);
	//vec3 zAxis(view[0][2], view[1][2], view[2][2]);
	//
	//view[3][0] = -dot(xAxis, transform->position);
	//view[3][1] = -dot(yAxis, transform->position);
	//view[3][2] = -dot(zAxis, transform->position);
	//
	//return view;

	return view;
}
