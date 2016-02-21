#include "MathUtil.h"

float MathUtil::RoundToNearest(float num, float nearest)
{
	return roundf(num / nearest) * nearest;
}
