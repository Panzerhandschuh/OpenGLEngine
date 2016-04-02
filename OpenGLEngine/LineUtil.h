#ifndef LINEUTIL_H
#define LINEUTIL_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

using namespace glm;

class LineUtil
{
public:
	static void DrawRay(vec3 pos, vec3 dir, vec3 color = vec3(1.0f, 1.0f, 1.0f));
	static void DrawLine(vec3 v1, vec3 v2, vec3 color = vec3(1.0f, 1.0f, 1.0f));
};

#endif