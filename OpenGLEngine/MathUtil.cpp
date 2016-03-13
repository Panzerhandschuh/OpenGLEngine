#include "MathUtil.h"

float MathUtil::RoundToNearest(float num, float nearest)
{
	return roundf(num / nearest) * nearest;
}

float MathUtil::Smoothstep(float start, float end, float t)
{
	float x = Clamp((t - start) / (end - start), 0.0f, 1.0f);
	return x * x * (3 - 2 * x);
}

float MathUtil::Clamp(float val, float min, float max)
{
	return fmin(max, fmax(val, min));
}
