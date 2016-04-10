#ifndef QUATERNIONUTIL_H
#define QUATERNIONUTIL_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "LineUtil.h"

class QuaternionUtil
{
public:
	static glm::vec3 GetRight(glm::quat rot);
	static glm::vec3 GetUp(glm::quat rot);
	static glm::vec3 GetForward(glm::quat rot);
	static glm::quat LookRotation(glm::vec3 forward, glm::vec3 up);
};

#endif