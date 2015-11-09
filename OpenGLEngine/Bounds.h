#ifndef BOUNDS_H
#define BOUNDS_H

#include "Model.h"

class Bounds
{
public:
	glm::vec3 size;
	glm::vec3 center;

	Bounds(Model& model);
	Bounds(Mesh& mesh);
};

#endif