#ifndef LINEUTIL_H
#define LINEUTIL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

using namespace glm;

class LineUtil
{
public:
	static void DrawRay(vec3 pos, vec3 dir);
	static void DrawLine(vec3 v1, vec3 v2);
};

#endif