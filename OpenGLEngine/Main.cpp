#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "BezierMesh.h"

GLuint screenWidth = 800;
GLuint screenHeight = 600;
GLfloat lastFrame = 0.0f;
GLfloat deltaTime = 0.0f;
Camera camera;

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
	glfwSwapInterval(0); // Disable vsync

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return 1;
	}

	InputManager::Init(window);

	// OpenGL options
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Initialize camera
	camera.position = glm::vec3(0.0f, 2.0f, 3.0f);

	// Create bezier meshes
	BezierCurve curve1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(4.0f, 0.0f, -4.0f), glm::vec3(8.0f, 0.0f, -4.0f));
	BezierMesh bMesh1(curve1);
	BezierCurve curve2(glm::vec3(8.0f, 0.0f, -4.0f), glm::vec3(10.0f, 0.0f, -4.0f), glm::vec3(12.0f, 0.0f, -6.0f), glm::vec3(12.0f, 0.0f, -8.0f));
	BezierMesh bMesh2(curve2);
	BezierCurve curve3(glm::vec3(12.0f, 0.0f, -8.0f), glm::vec3(12.0f, 0.0f, -16.0f), glm::vec3(4.0f, 0.0f, -6.0f), glm::vec3(0.0f, 0.0f, -6.0f));
	BezierMesh bMesh3(curve3);
	BezierCurve curve4(glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(-6.0f, 0.0f, -6.0f), glm::vec3(-6.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0.0f));
	BezierMesh bMesh4(curve4);

	// Create model
	Model monkey("suzanne.obj");

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Create shader
	Shader shader("lit-vert.glsl", "lit-frag.glsl");

	while (!glfwWindowShouldClose(window))
	{
		// Get frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Update keyboard/mouse input
		InputManager::Update();

		// Render settings
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Update camera
		//camera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
		camera.Update(deltaTime);

		// Create model matrix
		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::quarter_pi<GLfloat>(), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 projection;
		projection = glm::perspective(glm::quarter_pi<GLfloat>(), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 1000.0f);

		// Draw objects
		shader.Use();
		shader.SetUniform("model", model);
		shader.SetUniform("view", camera.view);
		shader.SetUniform("projection", projection);
		shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("ambient", 0.1f, 0.1f, 0.1f);
		shader.SetUniform("lightColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("lightDir", 1.0f, 1.0f, 1.0f);
		bMesh1.mesh.Draw();
		bMesh2.mesh.Draw();
		bMesh3.mesh.Draw();
		bMesh4.mesh.Draw();

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // Translate model upwards
		shader.SetUniform("model", model);
		monkey.Draw();

		// Draw output onto the window
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}