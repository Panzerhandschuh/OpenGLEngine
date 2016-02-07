#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "Model.h"
#include "Shader.h"
#include "Bounds.h"
#include "BezierCurve.h"
#include "InputManager.h"
#include "Physics.h"
#include "LineUtil.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace glm;

#define DEFORM_AXIS 2
#define MAX_PATH_VERTS 65535

class BezierPath
{
public:
	BezierPath(Model& sourceModel, BezierCurve& curve);
	~BezierPath();
	void Update(GLfloat deltaTime);
	void DeformPath();
	void Draw(Shader& shader);

private:
	BezierCurve curve;
	Model* sourceModel;
	Model pathModel;
	Model* pointModel;
	//vec3* selectedPoint = 0;
	std::vector<vec3*> selectedPoints;
	std::vector<vec3> pointOffsets; // Offset from plane to object positions
	vec3 planePos;
	bool useHeightPlane;

	int GetNumSegments();
	bool RaycastPoints(RaycastHit& hit, std::vector<vec3*>& selectedPoints);
};

#endif