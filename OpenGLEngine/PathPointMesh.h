#ifndef PATHPOINTMESH_H
#define PATHPOINTMESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "PathPoint.h"
#include "Bounds.h"
#include "QuaternionUtil.h"
#include "CrossSection.h"

#define DEFORM_AXIS 2
#define MAX_PATH_VERTS 65535

class PathPointMesh : public PathPoint
{
public:
	Model* lodMin;
	Model* lodMid;
	Model* lodMax;
	std::vector<CrossSection> crossSectionsMin;
	std::vector<CrossSection> crossSectionsMid;
	std::vector<CrossSection> crossSectionsMax;

	void Draw(Shader& shader);
	void Init(Model& sourceModel, glm::vec3 pos, glm::vec3 dir);
	void Init(Model& lodMin, Model& lodMid, Model& lodMax, glm::vec3 pos, glm::vec3 dir);
	void DeformPath();

private:
	Model* pointModel;
	float lodMinCurvature = 0.04f; // Min curvature for low level of detail
	float lodMaxCurvature = 0.8f; // Min curvature for high level of detail
	float lodMinTwist = 0.4f; // Min twist (in radians) for low level of detail
	float lodMaxTwist = 1.5f; // Max twist (in radians) for high level of detail

	int GetNumSegments(float curveLength);
};

#endif