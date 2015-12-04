#ifndef PATHPOINT_H
#define PATHPOINT_H

#include <glm/glm.hpp>
#include "Component.h"

class PathPoint : public Component
{
public:
	PathPoint* prev;
	PathPoint* next;
};

#endif