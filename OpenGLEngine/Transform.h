#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"

class Transform : public Component
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};


#endif