#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Engine.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "BezierMesh.h"
#include "BezierPath.h"
#include "Debug.h"

using namespace glm;

GLfloat lastFrame = 0.0f;
GLfloat deltaTime = 0.0f;

int main()
{
	glfwInit();

	// Configure window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(Engine::screenWidth, Engine::screenHeight, "OpenGL Tutorial", 0, 0);
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
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Initialize camera
	Camera::main.position = glm::vec3(0.0f, 2.0f, 3.0f);

	// Create bezier meshes
	BezierCurve curve1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(8.0f, 4.0f, -8.0f), glm::vec3(16.0f, 4.0f, -8.0f));
	//BezierMesh bMesh1(curve1);
	BezierCurve curve2(glm::vec3(16.0f, 4.0f, -8.0f), glm::vec3(20.0f, 4.0f, -8.0f), glm::vec3(24.0f, 0.0f, -12.0f), glm::vec3(24.0f, 0.0f, -16.0f));
	//BezierMesh bMesh2(curve2);
	BezierCurve curve3(glm::vec3(24.0f, 0.0f, -16.0f), glm::vec3(24.0f, 0.0f, -32.0f), glm::vec3(8.0f, 0.0f, -12.0f), glm::vec3(0.0f, 0.0f, -12.0f));
	//BezierMesh bMesh3(curve3);
	BezierCurve curve4(glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(-12.0f, 0.0f, -12.0f), glm::vec3(-12.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0.0f));
	//BezierMesh bMesh4(curve4);

	// Create model
	Model track("CoasterTrack.FBX");

	// Create bezier path
	BezierPath path1(track, curve1);
	path1.DeformPath();
	BezierPath path2(track, curve2);
	path2.DeformPath();
	BezierPath path3(track, curve3);
	path3.DeformPath();
	BezierPath path4(track, curve4);
	path4.DeformPath();

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

		// Update camera
		Camera::main.Update(deltaTime);

		// Render settings
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update objects
		path1.Update(deltaTime);
		path2.Update(deltaTime);
		path3.Update(deltaTime);
		path4.Update(deltaTime);

		// Create model matrix
		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::quarter_pi<GLfloat>(), glm::vec3(0.5f, 1.0f, 0.0f));

		// Draw objects
		shader.Use();
		shader.SetUniform("model", model);
		shader.SetUniform("view", Camera::main.view);
		shader.SetUniform("projection", Camera::main.proj);
		shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("ambient", 0.1f, 0.1f, 0.1f);
		shader.SetUniform("lightColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("lightDir", 1.0f, 1.0f, 1.0f);
		//bMesh1.Draw();
		//bMesh2.Draw();
		//bMesh3.Draw();
		//bMesh4.Draw();
		path1.Draw(shader);
		path2.Draw(shader);
		path3.Draw(shader);
		path4.Draw(shader);

		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // Translate model upwards
		//shader.SetUniform("model", model);
		//track.Draw();

		// Draw output onto the window
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}