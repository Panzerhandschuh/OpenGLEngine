#ifndef PATHPOINTSHAPES_H
#define PATHPOINTSHAPES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "PathPoint.h"
#include "PathShape.h"
#include "QuaternionUtil.h"

class PathPointShapes : public PathPoint
{
public:
	std::vector<PathShape> shapes;

	void Draw(Shader& shader);
	void Init(std::vector<PathShape>& shapes, glm::vec3 pos, glm::vec3 dir);
	void DeformPath();

private:
	Model* pointModel;
};

#endif