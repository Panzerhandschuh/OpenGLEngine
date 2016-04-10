#include "BezierCurve.h"

using namespace std;
using namespace glm;

BezierCurve::BezierCurve(vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
	SetControlPoints(p0, p1, p2, p3);
	CalcLength();
}

void BezierCurve::SetControlPoints(vec3 p0, vec3 p1, vec3 p2, vec3 p3)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

vec3 BezierCurve::GetPoint(float t)
{
	float t2 = t * t;
	float t3 = t2 * t;
	float tt1 = 1.0f - t;
	float tt2 = tt1 * tt1;
	float tt3 = tt2 * tt1;
	return (tt3 * p0) + (3.0f * t * tt2 * p1) + (3.0f * tt1 * t2 * p2) + (t3 * p3);
}

vec3 BezierCurve::Derivative(float t)
{
	float t2 = t * t;
	float tt1 = 1.0f - t;
	float tt2 = tt1 * tt1;
	return (-3.0f * tt2 * p0) + ((3.0f * tt2 - 6.0f * t * tt1) * p1) + ((6.0f * t * tt1 - 3.0f * t2) * p2) + (3.0f * t2 * p3);
}

glm::vec3 BezierCurve::Derivative2(float t)
{
	return 6.0f * (1.0f - t) * (p2 - 2.0f * p1 + p0) + 6.0f * t * (p3 - 2.0f * p2 + p1);
}

float BezierCurve::GetCurvature(float t)
{
	vec3 deriv = Derivative(t);
	vec3 deriv2 = Derivative2(t);
	return length(cross(deriv, deriv2)) / powf(length(deriv), 3.0f);
}

float BezierCurve::GetMaxCurvature()
{
	float max = numeric_limits<float>::min();
	for (int i = 0; i < NUM_STEPS; i++)
	{
		float t = (float)i / (float)(NUM_STEPS - 1);
		float curvature = GetCurvature(t);
		if (curvature > max)
			max = curvature;
	}

	return max;
}

float BezierCurve::GetLength()
{
	return arcLengths[arcLengths.size() - 1];
}

// Returns the arc length given a [0,1] normalized t-value
float BezierCurve::GetLengthParameter(float t)
{
	float targetLength = t * arcLengths[arcLengths.size() - 1];

	// Find largest arc length that is less than the target length
	int first = 0;
	int last = arcLengths.size() - 1;
	while (first <= last)
	{
		int mid = (first + last) / 2;
		if (arcLengths[mid] > targetLength)
			last = mid - 1;
		else
			first = mid + 1;
	}

	int index = first - 1;
	if (arcLengths[index] == targetLength) // Found an exact match, don't need to interpolate
		return index / (float)(arcLengths.size() - 1);

	// Interpolate between the two points
	float start = arcLengths[index];
	float end = arcLengths[index + 1];
	float length = end - start;
	float fraction = (targetLength - start) / length;
	return (index + fraction) / (float)(arcLengths.size() - 1);

	//float tIndex = t * (arcLengths.size() - 1);
	//int index = floor(tIndex);
	//if (index == 0)
	//	return arcLengths[0];
	//if (index == arcLengths.size() - 1)
	//	return arcLengths[arcLengths.size() - 1];
	
	//float start = arcLengths[index];
	//float end = arcLengths[index + 1];
	//return start + (end - start) * (tIndex - index);

	//return mix(arcLengths[index], arcLengths[index + 1], tIndex - index);
}

// Approximate curve length by adding up the straight line distances at fixed intervals of t
void BezierCurve::CalcLength()
{
	//float cpLength = distance(p0, p1) + distance(p1, p2) + distance(p2, p3);
	//int numSteps = ceil(cpLength * STEP_SCALE);
	//if (numSteps == 0)
	//	numSteps = 1;
	//cout << "CP Length: " << cpLength << endl;
	//cout << "Steps: " << numSteps << endl;
	arcLengths.resize(NUM_STEPS);
	arcLengths[0] = 0.0f;

	vec3 start = p0;
	for (int i = 1; i < NUM_STEPS; i++)
	{
		float t = (float)i / (float)(NUM_STEPS - 1);
		vec3 end = GetPoint(t);
		float length = distance(start, end);
		arcLengths[i] = arcLengths[i - 1] + length;

		start = end;
	}
}
