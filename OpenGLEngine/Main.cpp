#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Engine.h"
#include "EntityManager.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "BezierMesh.h"
#include "BezierPath.h"
#include "PathPointMesh.h"
#include "PathPointShapes.h"
#include "LineUtil.h"
#include "CrossSection.h"
#include "MeshExporter.h"
#include "ShapeUtil.h"
#include "LineUtil.h"

using namespace glm;
using namespace std;

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
	glClearColor(0.172549f, 0.235294117647f, 0.262745098f, 1.0f);

	// Initialize camera
	Camera::main.position = glm::vec3(0.0f, 2.0f, 3.0f);

	// Create bezier meshes
	//BezierCurve curve1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(8.0f, 4.0f, -8.0f), glm::vec3(16.0f, 4.0f, -8.0f));
	//BezierMesh bMesh1(curve1);
	//BezierCurve curve2(glm::vec3(16.0f, 4.0f, -8.0f), glm::vec3(20.0f, 4.0f, -8.0f), glm::vec3(24.0f, 0.0f, -12.0f), glm::vec3(24.0f, 0.0f, -16.0f));
	//BezierMesh bMesh2(curve2);
	//BezierCurve curve3(glm::vec3(24.0f, 0.0f, -16.0f), glm::vec3(24.0f, 0.0f, -32.0f), glm::vec3(8.0f, 0.0f, -12.0f), glm::vec3(0.0f, 0.0f, -12.0f));
	//BezierMesh bMesh3(curve3);
	//BezierCurve curve4(glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(-12.0f, 0.0f, -12.0f), glm::vec3(-12.0f, 0.0f, 0), glm::vec3(0.0f, 0.0f, 0.0f));
	//BezierMesh bMesh4(curve4);

	// Create bezier path
	//BezierPath point1(track, curve1);
	//point1.DeformPath();
	//BezierPath point2(track, curve2);
	//point2.DeformPath();
	//BezierPath path3(track, curve3);
	//path3.DeformPath();
	//BezierPath path4(track, curve4);
	//path4.DeformPath();

	Entity* managers = EntityManager::CreateEntity();
	managers->AddComponent<SelectionManager>();

	// Create model
	Model track("CoasterTrack.FBX");
	vector<CrossSection> crossSections = CrossSection::GetCrossSections(*track.meshes[0]);

	// Create shapes
	Shape shape = ShapeUtil::CreateCircle(1.0f, 32);
	PathShape pathShape(shape, vec2(0.0f, 0.0f));
	vector<PathShape> shapes = { pathShape };

	Entity* ent1 = EntityManager::CreateEntity();
	//PathPointMesh* point1 = ent1->AddComponent<PathPointMesh>();
	//point1->Init(track, crossSections, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	PathPointShapes* point1 = ent1->AddComponent<PathPointShapes>();
	point1->Init(shapes, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	Entity* ent2 = EntityManager::CreateEntity();
	//PathPointMesh* point2 = ent2->AddComponent<PathPointMesh>();
	//point2->Init(track, crossSections, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	PathPointShapes* point2 = ent2->AddComponent<PathPointShapes>();
	point2->Init(shapes, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	point1->next = point2;
	point2->prev = point1;
	point1->DeformPath();

	//Entity* point = EntityManager::CreateEntity();
	//point->AddComponent<Transform>();
	//Model* model = point->AddComponent<Model>();
	//model->LoadModel("Sphere.FBX");
	//point->AddComponent<PathPointMesh>();

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
		//cout << 1.0f / deltaTime << endl;

		// Update keyboard/mouse input
		InputManager::Update();

		// Save scene to obj
		if (InputManager::GetKey(GLFW_KEY_LEFT_CONTROL) && InputManager::GetKeyDown(GLFW_KEY_S))
		{
			int count = 0;
			for (int i = 0; i < EntityManager::entities.size(); i++)
			{
				PathPoint* point = EntityManager::entities[i]->GetComponent<PathPoint>();
				if (point && !point->IsEnd())
				{
					// Save mesh
					MeshExporter::Export("Mesh" + to_string(count) + ".obj", *point->pathModel.meshes[0]);
					count++;
				}
			}
		}

		// Update camera
		Camera::main.Update(deltaTime);

		// Render settings
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update objects
		EntityManager::UpdateAll(deltaTime);
		//point1.Update(deltaTime);
		//point2.Update(deltaTime);
		//path3.Update(deltaTime);
		//path4.Update(deltaTime);

		// Create model matrix
		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, glm::quarter_pi<GLfloat>(), glm::vec3(0.5f, 1.0f, 0.0f));
		point1->DeformPath();

		// Draw objects
		shader.Use();
		shader.SetUniform("model", model);
		shader.SetUniform("view", Camera::main.view);
		shader.SetUniform("projection", Camera::main.proj);
		shader.SetUniform("objectColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("ambient", 0.1f, 0.1f, 0.1f);
		shader.SetUniform("lightColor", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("lightDir", 1.0f, 1.0f, 1.0f);
		shader.SetUniform("viewPos", Camera::main.position);
		EntityManager::DrawAll(shader);
		//bMesh1.Draw();
		//bMesh2.Draw();
		//bMesh3.Draw();
		//bMesh4.Draw();
		//point1.Draw(shader);
		//point2.Draw(shader);
		//path3.Draw(shader);
		//path4.Draw(shader);

		//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // Translate model upwards
		//shader.SetUniform("model", model);
		//track.Draw();

		// Draw output onto the window
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}