#ifndef PATHPOINT_H
#define PATHPOINT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Component.h"
#include "Transform.h"
#include "BezierCurve.h"
#include "Model.h"
#include "Shader.h"
#include "InputManager.h"
#include "SelectionManager.h"
#include "Bounds.h"
#include "PathPointHandle.h"
#include "QuaternionUtil.h"
#include "LineUtil.h"

class PathPoint : public Component
{
public:
	PathPoint* prev;
	PathPoint* next;
	float angle = 0.0f;
	Transform* startHandle;
	Transform* endHandle;
	Model pathModel;

	virtual void Start();
	~PathPoint();
	void Update(GLfloat deltaTime);
	virtual void Draw(Shader& shader);
	void Init(glm::vec3 pos, glm::vec3 dir);
	virtual void DeformPath() = 0;
	void UpdateHandles(glm::vec3 moveDelta);
	bool IsStart();
	bool IsEnd();
	bool IsIntermediate();
	glm::vec3 GetDirection();

private:
	Model* pointModel;
};

#endif
