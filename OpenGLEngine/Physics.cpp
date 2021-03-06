#include "Physics.h"

using namespace std;
using namespace glm;

RaycastHit::RaycastHit()
{
	entity = 0;
}

vec3 Physics::GetRayFromMouse()
{
	// Screen space
	vec2 mousePos = InputManager::mousePos;
	vec2 rayScreen;
	rayScreen.x = (2.0f * mousePos.x) / Engine::screenWidth - 1.0f;
	rayScreen.y = 1.0f - (2.0f * mousePos.y) / Engine::screenHeight;

	// Clip space
	vec4 rayClip = vec4(rayScreen.x, rayScreen.y, -1.0, 1.0);

	// Eye space
	vec4 rayEye = inverse(Camera::main->proj) * rayClip;
	rayEye = vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// World space
	vec3 rayWorld = vec3(inverse(Camera::main->GetView()) * rayEye);
	return normalize(rayWorld);
}

bool Physics::RaycastScene(vec3 rayPos, vec3 rayDir, RaycastHit& hit, int layer)
{
	float minDist = numeric_limits<float>::max();
	vector<Entity*> entities = EntityManager::entities;
	for (int i = 0; i < entities.size(); i++)
	{
		Entity* ent = entities[i];
		if (!ent->enabled || ent->layer != layer)
			continue;

		SphereCollider* collider = ent->GetComponent<SphereCollider>();
		if (collider) // Only raycast sphere colliders
		{
			if (Physics::RaycastSphere(rayPos, rayDir, ent->transform->position, collider->radius, hit))
			{
				cout << "Hit Sphere: " << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << endl;
				if (hit.distance < minDist)
				{
					minDist = hit.distance;
					hit.entity = ent;
				}
			}
		}
	}

	return (hit.entity != 0);
}

bool Physics::RaycastPlane(vec3 rayPos, vec3 rayDir, vec3 planePos, vec3 planeNorm, RaycastHit& hit)
{
	float dot1 = dot(rayDir, planeNorm);
	if (dot1 == 0.0f)
		return false;

	float dist = dot(-planePos, planeNorm);
	float dot2 = dot(rayPos, planeNorm);
	float t = -(dot2 + dist) / dot1;
	if (t < 0.0f)
		return false;

	hit.distance = t;
	hit.point = rayPos + rayDir * t;
	return true;
}

bool Physics::RaycastSphere(vec3 rayPos, vec3 rayDir, vec3 spherePos, float sphereRadius, RaycastHit& hit)
{
	// Find quadratic equation
	vec3 toSphere = rayPos - spherePos;
	float b = dot(rayDir, toSphere);
	float c = dot(toSphere, toSphere) - sphereRadius * sphereRadius;
	float bSquaredMinusC = b * b - c;

	// Check for imaginary answer (ray completely misses sphere)
	if (bSquaredMinusC < 0.0f)
		return false;

	// Check for ray hitting twice (in and out of the sphere)
	if (bSquaredMinusC > 0.0f)
	{
		// Get the 2 intersection distances along ray
		float root1 = -b + sqrt(bSquaredMinusC);
		float root2 = -b - sqrt(bSquaredMinusC);
		float t = root2;

		// If behind viewer, throw one or both away
		if (root1 < 0.0f)
		{
			if (root2 < 0.0f)
				return false;
		}
		else if (root2 < 0.0f)
			t = root1;

		hit.distance = t;
		hit.point = rayPos + rayDir * t;
		return true;
	}

	// Check for ray hitting once (skimming the surface)
	if (bSquaredMinusC == 0.0f)
	{
		// If behind viewer, throw away
		float t = -b + sqrt(bSquaredMinusC);
		if (t < 0.0f)
			return false;

		hit.distance = t;
		hit.point = rayPos + rayDir * t;
		return true;
	}

	// Note: Could also check if ray origin is inside sphere radius
	return false;
}