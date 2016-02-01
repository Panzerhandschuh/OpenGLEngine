#include "SelectionManager.h"

using namespace std;
using namespace glm;

SelectionManager* SelectionManager::instance;

void SelectionManager::Start()
{
	instance = this;
}

void SelectionManager::Update(GLfloat deltaTime)
{
	if (InputManager::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		RaycastHit hit;
		vec3 rayPos = Camera::main.position;
		vec3 rayDir = Physics::GetRayFromMouse();
		if (Physics::RaycastScene(rayPos, rayDir, hit))
		{
			selectedEnt = hit.entity;
			planePos = hit.point;
			useHeightPlane = InputManager::GetKey(GLFW_KEY_LEFT_CONTROL);
			selectionOffset = selectedEnt->transform->position - planePos;
			//cout << "dist: " << hit.distance << endl;
		}
		else
			selectedEnt = 0;
	}

	if (selectedEnt && InputManager::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) // Move selected entity
	{
		vec3 rayPos = Camera::main.position;
		vec3 rayDir = Physics::GetRayFromMouse();
		vec3 planeNorm = (useHeightPlane) ? Camera::main.GetForward() : vec3(0.0f, 1.0f, 0.0f);
		RaycastHit hit;
		if (Physics::RaycastPlane(rayPos, rayDir, planePos, planeNorm, hit))
		{
			//cout << "dist: " << hit.distance << endl;
			if (useHeightPlane)
			{
				hit.point.x = planePos.x;
				hit.point.z = planePos.z;
			}

			selectedEnt->transform->position = hit.point + selectionOffset;

			// Update paths if possible
			PathPoint* point = selectedEnt->GetComponent<PathPoint>();
			if (point)
				point->DeformPath();
		}
	}
}