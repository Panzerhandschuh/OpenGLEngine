#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H

#include <glm\glm.hpp>
#include "Component.h"
#include "BezierCurve.h"
#include "PathPoint.h"

class PathFollower : public Component
{
public:
	float minSpeed = 8.0f;
	float velocity = 0.0f;
	void Update(GLfloat deltaTime);
	void Init(PathPoint* point, glm::vec3 offset = glm::vec3());
	void StartFollow();

private:
	PathPoint* startPoint;
	PathPoint* currentPoint;
	glm::vec3 offset;
	float currentPos = 0.0f; // Current position on curve
};

#endif