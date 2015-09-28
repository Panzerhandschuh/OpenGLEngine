#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void UpdateCamera();

GLuint screenWidth = 800;
GLuint screenHeight = 600;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera camera;

bool keys[1024];

int main()
{
	glfwInit();

	// Configure window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Tutorial", 0, 0);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return 1;
	}

	// Input callbacks
	glfwSetKeyCallback(window, KeyCallback);

	// OpenGL options
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	GLfloat verts[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f
	};

	GLuint indices[] = {
		0, 3, 2,
		2, 1, 0,
		4, 5, 6,
		6, 7, 4,
		7, 3, 0,
		0, 4, 7,
		6, 5, 1,
		1, 2, 6,
		0, 1, 5,
		5, 4, 0,
		6, 2, 3,
		3, 7, 6
	};

	GLfloat texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	// Initialize camera
	camera.position = glm::vec3(0.0f, 2.0f, 3.0f);

	// Create triangle geometry
	Mesh mesh;
	mesh.SetVertices(verts, 8);
	mesh.SetIndices(indices, 36);
	mesh.SetTexCoords(texCoords, 8);
	mesh.SetTexture("container.jpg");
	mesh.SetVertexColors(colors, 8);

	// Create shader
	Shader shader("vert.glsl", "frag.glsl");

	while (!glfwWindowShouldClose(window))
	{
		// Get frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check for keyboard/mouse input
		glfwPollEvents();

		// Render settings
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Update camera
		UpdateCamera();

		// Create model matrix
		glm::mat4 model;
		//model = glm::rotate(model, glm::quarter_pi<GLfloat>(), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 projection;
		projection = glm::perspective(glm::quarter_pi<GLfloat>(), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

		// Draw triangle
		shader.Use();
		shader.SetUniform("model", model);
		shader.SetUniform("view", camera.view);
		shader.SetUniform("projection", projection);
		mesh.Draw();

		// Draw output onto the window
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Close the window when the user presses escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Track keyboard input
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void UpdateCamera()
{
	const GLfloat camSpeed = 8.0f;

	if (keys[GLFW_KEY_W])
		camera.position += camSpeed * -camera.GetForward() * deltaTime;
	if (keys[GLFW_KEY_S])
		camera.position += camSpeed * camera.GetForward() * deltaTime;
	if (keys[GLFW_KEY_A])
		camera.position += camSpeed * -camera.GetRight() * deltaTime;
	if (keys[GLFW_KEY_D])
		camera.position += camSpeed * camera.GetRight() * deltaTime;
	if (keys[GLFW_KEY_Q])
		camera.position += camSpeed * -camera.GetUp() * deltaTime;
	if (keys[GLFW_KEY_E])
		camera.position += camSpeed * camera.GetUp() * deltaTime;

	camera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
}