#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

using namespace glm;

class Debug
{
public:
	static void DrawRay(vec3 pos, vec3 dir);
	static void DrawLine(vec3 v1, vec3 v2);
};

#endif