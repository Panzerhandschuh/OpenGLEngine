#include "MathUtil.h"

float MathUtil::RoundToNearest(float num, float nearest)
{
	return roundf(num / nearest) * nearest;
}

float MathUtil::Clamp(float val, float min, float max)
{
	return fmin(max, fmax(val, min));
}
