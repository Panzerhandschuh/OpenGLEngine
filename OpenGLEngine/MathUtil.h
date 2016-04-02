#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <math.h>

class MathUtil
{
public:
	static float RoundToNearest(float num, float nearest);
	static float Clamp(float min, float max, float val);
};

#endif