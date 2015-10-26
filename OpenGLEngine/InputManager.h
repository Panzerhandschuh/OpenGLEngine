#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager
{
public:
	static bool keys[1024];
	static glm::vec2 mouseDelta;
	static GLfloat mouseSensitivity;

	static void Init(GLFWwindow* window);
	static void Update();

private:
	static GLFWwindow* window;
	static bool firstMouse;
	static glm::vec2 lastMouse;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	//static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif