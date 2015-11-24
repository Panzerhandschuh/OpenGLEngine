#include "InputManager.h"

glm::vec2 InputManager::mousePos;
glm::vec2 InputManager::mouseDelta;

GLFWwindow* InputManager::window;
bool InputManager::firstMouse = true;
bool InputManager::keys[GLFW_KEY_LAST];
bool InputManager::keysDown[GLFW_KEY_LAST];
bool InputManager::mouseButtons[GLFW_MOUSE_BUTTON_LAST];
bool InputManager::mouseButtonsDown[GLFW_MOUSE_BUTTON_LAST];

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
	// Clear keys pressed down
	ResetDown();

	// Check for input
	glfwPollEvents();

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (firstMouse)
	{
		mousePos.x = (GLfloat)xpos;
		mousePos.y = (GLfloat)ypos;
		firstMouse = false;
	}
	
	mouseDelta.x = (GLfloat)xpos - mousePos.x;
	mouseDelta.y = mousePos.y - (GLfloat)ypos; // Reversed since y-coordinates go from bottom to left

	mousePos.x = (GLfloat)xpos;
	mousePos.y = (GLfloat)ypos;
}

bool InputManager::GetKey(int key)
{
	return keys[key];
}

bool InputManager::GetKeyDown(int key)
{
	return keysDown[key];
}

bool InputManager::GetMouseButton(int button)
{
	return mouseButtons[button];
}

bool InputManager::GetMouseButtonDown(int button)
{
	return mouseButtonsDown[button];
}

void InputManager::SetCursorVisibility(bool isVisible)
{
	int cursorMode = (isVisible) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN;
	glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
}

void InputManager::ResetDown()
{
	std::fill(keysDown, keysDown + GLFW_KEY_LAST, 0);
	std::fill(mouseButtonsDown, mouseButtonsDown + GLFW_MOUSE_BUTTON_LAST, 0);
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
		{
			keys[key] = true;
			keysDown[key] = true;
		}
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST)
	{
		if (action == GLFW_PRESS)
		{
			mouseButtons[button] = true;
			mouseButtonsDown[button] = true;
		}
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