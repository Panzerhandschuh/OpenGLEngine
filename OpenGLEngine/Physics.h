#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include "Camera.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "SphereCollider.h"

class RaycastHit
{
public:
	Entity* entity;
	glm::vec3 point;
	float distance;

	RaycastHit();
};

class Physics
{
public:
	static glm::vec3 GetRayFromMouse();

	static bool RaycastScene(glm::vec3 rayPos, glm::vec3 rayDir, RaycastHit& hit);
	static bool RaycastPlane(glm::vec3 rayPos, glm::vec3 rayDir, glm::vec3 planePos, glm::vec3 planeNorm, RaycastHit& hit);
	static bool RaycastSphere(glm::vec3 rayPos, glm::vec3 rayDir, glm::vec3 spherePos, float sphereRadius, RaycastHit& hit);
};

#endif