#ifndef PATHPOINTHANDLE_H
#define PATHPOINTHANDLE_H

#include <glm/glm.hpp>
#include "Component.h"
#include "Entity.h"
#include "SphereCollider.h"

class PathPointHandle : public Component
{
public:
	Transform* otherHandle;

	void Start();
	void UpdatePosition(glm::vec3 moveDelta);
};

#endif