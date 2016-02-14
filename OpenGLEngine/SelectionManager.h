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
	Entity* selectedHandle;

	void Start();
	void Update(GLfloat deltaTime);

private:
	glm::vec3 selectionOffset; // Offset from plane to entity position
	glm::vec3 planePos;
	glm::vec3 lastMove;
	bool useHeightPlane;

	void HandleSelection(RaycastHit& hit);
	void RotateSelection();
	void MoveSelection();
	void CreatePathPoint();
};

#endif