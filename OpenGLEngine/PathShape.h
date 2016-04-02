#ifndef PATHSHAPE_H
#define PATHSHAPE_H

#include <glm\glm.hpp>
#include "Shape.h"

class PathShape
{
public:
	Shape shape;
	glm::vec2 offset;

	PathShape(Shape shape, glm::vec2 offset);
};

#endif