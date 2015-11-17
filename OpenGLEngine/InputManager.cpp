#include "InputManager.h"

bool InputManager::keys[GLFW_KEY_LAST];
bool InputManager::mouseButtons[GLFW_MOUSE_BUTTON_LAST];
glm::vec2 InputManager::mouseDelta;
GLfloat InputManager::mouseSensitivity = 0.25f;

GLFWwindow* InputManager::window;
bool InputManager::firstMouse = true;
glm::vec2 InputManager::lastMouse;

void InputManager::Init(GLFWwindow* window)
{
	InputManager::window = window;

	// Input callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	//glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::Update()
{
	glfwPollEvents();

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (firstMouse)
	{
		lastMouse.x = (GLfloat)xpos;
		lastMouse.y = (GLfloat)ypos;
		firstMouse = false;
	}
	
	mouseDelta.x = (GLfloat)xpos - lastMouse.x;
	mouseDelta.y = lastMouse.y - (GLfloat)ypos; // Reversed since y-coordinates go from bottom to left
	lastMouse.x = (GLfloat)xpos;
	lastMouse.y = (GLfloat)ypos;
	
	mouseDelta *= mouseSensitivity;
	mouseDelta *= mouseSensitivity;
}

void InputManager::SetCursorVisibility(bool isVisible)
{
	int cursorMode = (isVisible) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN;
	glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Close the window when the user presses escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Track keyboard input
	if (key >= 0 && key < GLFW_KEY_LAST)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST)
	{
		if (action == GLFW_PRESS)
			mouseButtons[button] = true;
		else if (action == GLFW_RELEASE)
			mouseButtons[button] = false;
	}
}

//void InputManager::MouseCallback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastMouse.x = (GLfloat)xpos;
//		lastMouse.y = (GLfloat)ypos;
//		firstMouse = false;
//	}
//
//	mouseDelta.x = (GLfloat)xpos - lastMouse.x;
//	mouseDelta.y = lastMouse.y - (GLfloat)ypos; // Reversed since y-coordinates go from bottom to left
//	lastMouse.x = (GLfloat)xpos;
//	lastMouse.y = (GLfloat)ypos;
//
//	mouseDelta *= mouseSensitivity;
//	mouseDelta *= mouseSensitivity;
//}