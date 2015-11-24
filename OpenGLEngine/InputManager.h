#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <algorithm>

class InputManager
{
public:
	static glm::vec2 mousePos;
	static glm::vec2 mouseDelta;

	static void Init(GLFWwindow* window);
	static void Update();
	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);
	static void SetCursorVisibility(bool isVisible);

private:
	static GLFWwindow* window;
	static bool firstMouse;
	static bool keys[GLFW_KEY_LAST];
	static bool keysDown[GLFW_KEY_LAST];
	static bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];
	static bool mouseButtonsDown[GLFW_MOUSE_BUTTON_LAST];

	static void ResetDown(); // Resets keys and mouse buttons down
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	//static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif