#ifndef PATHPOINT_H
#define PATHPOINT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "Transform.h"
#include "BezierCurve.h"
#include "Model.h"
#include "Shader.h"
#include "InputManager.h"
#include "SelectionManager.h"
#include "Bounds.h"
#include "PathPointHandle.h"
#include "LineUtil.h"

#define DEFORM_AXIS 2
#define MAX_PATH_VERTS 65535

class PathPoint : public Component
{
public:
	PathPoint* prev;
	PathPoint* next;
	Transform* startHandle;
	Transform* endHandle;

	void Start();
	~PathPoint();
	void Update(GLfloat deltaTime);
	void Draw(Shader& shader);
	void Init(Model& sourceModel, glm::vec3 pos);
	void DeformPath();
	void UpdateHandles(glm::vec3 moveDelta);

private:
	Model* sourceModel;
	Model pathModel;
	Model* pointModel;

	int GetNumSegments(BezierCurve& curve);
};

#endif