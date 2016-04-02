#ifndef SHAPEUTIL_H
#define SHAPEUTIL_H

#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <vector>
#include "Shape.h"

class ShapeUtil
{
public:
	static Shape CreateCircle(float radius, int numSides);
};

#endif