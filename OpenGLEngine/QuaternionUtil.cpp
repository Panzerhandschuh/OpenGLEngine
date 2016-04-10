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

quat QuaternionUtil::LookRotation(glm::vec3 forward, glm::vec3 up)
{
	vec3 right = cross(forward, up);
	//mat4 rot = mat4(right.x, right.y, right.z, 0.0f, 
	//	up.x, up.y, up.z, 0.0f, 
	//	forward.x, forward.y, forward.z, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f);
	//mat4 rot = orientation(forward, up);
	mat3 rot(right, up, forward);

	//LineUtil::DrawRay(vec3(), up * 5.0f, vec3(0.0f, 1.0f, 0.0f));
	//LineUtil::DrawRay(vec3(), right * 5.0f, vec3(1.0f, 0.0f, 0.0f));
	//LineUtil::DrawRay(vec3(), forward * 5.0f, vec3(0.0f, 0.0f, 1.0f));
	
	return quat(rot);
}
