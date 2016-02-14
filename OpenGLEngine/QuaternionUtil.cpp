#include "QuaternionUtil.h"

using namespace glm;

vec3 QuaternionUtil::GetRight(quat rot)
{
	return vec3(1.0f - 2.0f * (rot.y * rot.y + rot.z * rot.z),
		2.0f * (rot.x * rot.y + rot.w * rot.z),
		2.0f * (rot.x * rot.z - rot.w * rot.y));
}

vec3 QuaternionUtil::GetUp(quat rot)
{
	return vec3(2.0f * (rot.x * rot.y - rot.w * rot.z),
		1.0f - 2.0f * (rot.x * rot.x + rot.z * rot.z),
		2.0f * (rot.y * rot.z + rot.w * rot.x));
}

vec3 QuaternionUtil::GetForward(quat rot)
{
	return vec3(2.0f * (rot.x * rot.z + rot.w * rot.y),
		2.0f * (rot.y * rot.x - rot.w * rot.x),
		1.0f - 2.0f * (rot.x * rot.x + rot.y * rot.y));
}
