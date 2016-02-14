#include "Transform.h"

using namespace glm;

vec3 Transform::GetRight()
{
	return QuaternionUtil::GetRight(rotation);
}

vec3 Transform::GetUp()
{
	return QuaternionUtil::GetUp(rotation);
}

vec3 Transform::GetForward()
{
	return QuaternionUtil::GetForward(rotation);
}
