#include "PathPointHandle.h"

void PathPointHandle::Start()
{
	SphereCollider* collider = entity->AddComponent<SphereCollider>();
	collider->radius = 0.5f;
	entity->layer = 1;
	entity->enabled = false;
}

void PathPointHandle::UpdatePosition(glm::vec3 moveDelta)
{
	otherHandle->position -= moveDelta;
}
