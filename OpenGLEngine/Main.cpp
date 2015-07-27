#define GLEW_STATIC

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Mesh.h"
#include "Shader.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	glfwInit();

	// Configure window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Tutorial", 0, 0);
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

	//GLfloat verts[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f, 0.5f, 0.0f
	//};

	GLfloat verts[] = {
		0.5f, 0.5f, 0.0f,	// Top right
		0.5f, -0.5f, 0.0f,	// Bottom right
		-0.5f, -0.5f, 0.0f,	// Bottom left
		-0.5f, 0.5f, 0.0f	// Top left 
	};

	GLuint indices[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};

	GLfloat texCoords[] = {
		1.0f, 1.0f,	// Top right
		1.0f, 0.0f,	// Bottom right
		0.0f, 0.0f,	// Bottom left
		0.0f, 1.0f	// Top left
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	// Create triangle geometry
	//Mesh mesh(verts, 4, indices, 6);
	Mesh mesh;
	mesh.SetVertices(verts, 4);
	mesh.SetIndices(indices, 6);
	mesh.SetTexCoords(texCoords, 4);
	mesh.SetTexture("container.jpg");
	mesh.SetVertexColors(colors, 4);

	// Create shader
	Shader shader("vert.glsl", "frag.glsl");

	while (!glfwWindowShouldClose(window))
	{
		// Check for keyboard/mouse input
		glfwPollEvents();

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

		// Draw triangle
		shader.Use();
		//shader.SetAttribute("myColor", 0.0f, 0.5f, 1.0f, 1.0f);
		//shader.SetAttribute("offset", 0.5f, 0.0f, 0.0f);
		mesh.Draw();

		// Draw output onto the window
		glfwSwapBuffers(window);
	}

	//glDeleteProgram(program);

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Close the window when the user presses escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}