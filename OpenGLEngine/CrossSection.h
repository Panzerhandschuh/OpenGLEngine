#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include <glm\glm.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include "Mesh.h"
#include "Bounds.h"
#include "MathUtil.h"

class FloatCompare
{
public:
	bool operator()(const float a, const float b);
};

class CrossSection
{
public:
	float tValue;
	std::vector<int> indices;

	CrossSection() { }
	CrossSection(float tValue, std::vector<int> indices);
	static std::vector<CrossSection> GetCrossSections(Mesh& mesh);
};

#endif