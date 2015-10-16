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
#include "BezierCurve.h"

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
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
	};

	GLfloat texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	GLfloat normals[] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Initialize camera
	camera.position = glm::vec3(0.0f, 2.0f, 3.0f);

	// Create bezier mesh
	BezierCurve curve(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(4.0f, 0.0f, -4.0f), glm::vec3(8.0f, 0.0f, -4.0f));
	const int numSegments = 20;
	const int numSlices = numSegments + 1;
	const int numVerts = 2 * numSlices;
	glm::vec3 bVerts[numVerts], bNorms[numVerts];
	for (int i = 0; i <= numSegments; i++)
	{
		float t = (float)i / (float)numSegments;
		glm::vec3 point = curve.GetPoint(t);
		glm::vec3 tang = glm::normalize(curve.GetTangent(t));
		glm::vec3 norm(0.0f, 1.0f, 0.0f);
		glm::vec3 biNorm = glm::cross(tang, norm);
		//std::cout << biNorm.x << ", " << biNorm.y << ", " << biNorm.z << std::endl;

		int index = i * 2; // Starting index of this slice
		bVerts[index] = point + biNorm;
		bVerts[index + 1] = point - biNorm;
		bNorms[index] = norm;
		bNorms[index + 1] = norm;
	}

	// Set up indices
	const int numIndices = 6 * numSegments;
	GLuint bIndices[numIndices];
	for (int i = 0; i < numSegments; i++)
	{
		int tIndex = i * 6; // Starting triangle index of this segment
		int vIndex = i * 2; // Starting vertex index of this segment

		bIndices[tIndex] = vIndex;
		bIndices[tIndex + 1] = vIndex + 1;
		bIndices[tIndex + 2] = vIndex + 2;

		bIndices[tIndex + 3] = vIndex + 1;
		bIndices[tIndex + 4] = vIndex + 3;
		bIndices[tIndex + 5] = vIndex + 2;
	}

	// Create triangle geometry
	Mesh mesh;
	mesh.SetVertices(bVerts, numVerts);
	mesh.SetIndices(bIndices, numIndices);
	mesh.SetNormals(bNorms, numVerts);
	//mesh.SetTexCoords(texCoords, 36);
	//mesh.SetTexture("container.jpg");
	//mesh.SetVertexColors(colors, 8);

	// Create shader
	Shader shader("lit-vert.glsl", "lit-frag.glsl");

	while (!glfwWindowShouldClose(window))
	{
		// Get frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
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
		shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("ambient", 0.1f, 0.1f, 0.1f);
		shader.SetUniform("lightColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("lightDir", 1.0f, 1.0f, 1.0f);
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