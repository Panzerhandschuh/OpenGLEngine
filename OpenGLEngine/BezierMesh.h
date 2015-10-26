#ifndef BEZIERMESH_H
#define BEZIERMESH_H

#include "Mesh.h"
#include "BezierCurve.h"
#include <glm/glm.hpp>

class BezierMesh
{
public:
	Mesh mesh;

	BezierMesh(BezierCurve& curve);
};

#endif