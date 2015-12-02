#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include "Camera.h"
#include "InputManager.h"

using namespace glm;

class RaycastHit
{
public:
	vec3 point;
	float distance;
};

class Physics
{
public:
	static vec3 GetRayFromMouse();
	static bool RaycastPlane(vec3 rayPos, vec3 rayDir, vec3 planePos, vec3 planeNorm, RaycastHit& hit);
	static bool RaycastSphere(vec3 rayPos, vec3 rayDir, vec3 spherePos, float sphereRadius, RaycastHit& hit);
};

#endif