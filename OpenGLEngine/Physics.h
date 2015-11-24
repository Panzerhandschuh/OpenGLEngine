#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include "Camera.h"
#include "InputManager.h"

using namespace glm;

class Physics
{
public:
	static vec3 GetRayFromMouse();
	static bool RaycastPlane(vec3 rayPos, vec3 rayDir, vec3 planePos, vec3 planeNorm, float& dist);
	static bool RaycastSphere(vec3 rayPos, vec3 rayDir, vec3 spherePos, float sphereRadius, float& dist);
};

#endif