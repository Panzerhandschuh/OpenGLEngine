#include "Physics.h"

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
	vec4 rayEye = inverse(Camera::main.proj) * rayClip;
	rayEye = vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// World space
	vec3 rayWorld = vec3(inverse(Camera::main.view) * rayEye);
	return normalize(rayWorld);
}

bool Physics::RaycastPlane(vec3 rayPos, vec3 rayDir, vec3 planePos, vec3 planeNorm, float& dist)
{
	return false;
}

bool Physics::RaycastSphere(vec3 rayPos, vec3 rayDir, vec3 spherePos, float sphereRadius, float& dist)
{
	// Find quadratic equation
	vec3 toSphere = rayPos - spherePos;
	float b = dot(rayDir, toSphere);
	float c = dot(toSphere, toSphere) - (sphereRadius * sphereRadius);
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
		dist = root2;

		// If behind viewer, throw one or both away
		if (root1 < 0.0f)
		{
			if (root2 < 0.0f)
				return false;
		}
		else if (root2 < 0.0f)
			dist = root1;

		return true;
	}

	// Check for ray hitting once (skimming the surface)
	if (bSquaredMinusC == 0.0f)
	{
		// If behind viewer, throw away
		float t = -b + sqrt(bSquaredMinusC);
		if (t < 0.0f)
			return false;

		dist = t;
		return true;
	}

	// Note: Could also check if ray origin is inside sphere radius
	return false;
}