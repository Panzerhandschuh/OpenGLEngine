#ifndef SHAPE_H
#define SHAPE_H

#include <glm\glm.hpp>
#include <vector>

class Shape
{
public:
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> normals;
	std::vector<float> us;
	std::vector<int> indices;
};

#endif