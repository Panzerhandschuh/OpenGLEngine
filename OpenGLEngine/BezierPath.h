#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "Model.h"
#include "Bounds.h"
#include "BezierCurve.h"

#define DEFORM_AXIS 2

class BezierPath
{
public:
	BezierPath(Model& sourceModel, BezierCurve& curve);
	void DeformPath();
	void Draw();

private:
	BezierCurve curve;
	Model* sourceModel;
	Model pathModel;
};

#endif