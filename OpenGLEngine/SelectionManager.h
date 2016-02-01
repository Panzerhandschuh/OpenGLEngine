#ifndef SELECTIONUTIL_H
#define SELECTIONUTIL_H

#include <glm\glm.hpp>
#include <vector>
#include "EntityManager.h"
#include "Physics.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "PathPoint.h"

class SelectionManager : public Component
{
public:
	static SelectionManager* instance;
	Entity* selectedEnt;

	void Start();
	void Update(GLfloat deltaTime);

private:
	glm::vec3 selectionOffset; // Offset from plane to entity position
	glm::vec3 planePos;
	bool useHeightPlane;
};

#endif