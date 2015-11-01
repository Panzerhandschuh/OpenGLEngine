#ifndef BEZIERMESH_H
#define BEZIERMESH_H

#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "BezierCurve.h"

using namespace std;

class BezierMesh
{
public:
	Mesh mesh;

	BezierMesh(BezierCurve& curve);
};

#endif