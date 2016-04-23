#ifndef LINEUTIL_H
#define LINEUTIL_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

class LineUtil
{
public:
	static void DrawRay(glm::vec3 pos, glm::vec3 dir, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	static void DrawLine(glm::vec3 v1, glm::vec3 v2, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif