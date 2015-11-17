#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager
{
public:
	static bool keys[GLFW_KEY_LAST];
	static bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];
	static glm::vec2 mouseDelta;
	static GLfloat mouseSensitivity;

	static void Init(GLFWwindow* window);
	static void Update();
	static void SetCursorVisibility(bool isVisible);

private:
	static GLFWwindow* window;
	static bool firstMouse;
	static glm::vec2 lastMouse;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	//static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif