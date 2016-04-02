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
	Model* sourceModel;
	std::vector<CrossSection> crossSections;

	void Draw(Shader& shader);
	void Init(Model& sourceModel, std::vector<CrossSection>& crossSections, glm::vec3 pos, glm::vec3 dir);
	void DeformPath();

private:
	Model* pointModel;

	int GetNumSegments(float curveLength);
};

#endif