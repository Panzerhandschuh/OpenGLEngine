#include "BezierCurve.h"

BezierCurve::BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	SetControlPoints(p0, p1, p2, p3);
}

void BezierCurve::SetControlPoints(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

glm::vec3 BezierCurve::GetPoint(float t)
{
	float t2 = t * t;
	float t3 = t2 * t;
	float tt1 = 1.0f - t;
	float tt2 = tt1 * tt1;
	float tt3 = tt2 * tt1;
	return (tt3 * p0) + (3.0f * t * tt2 * p1) + (3.0f * tt1 * t2 * p2) + (t3 * p3);
}

glm::vec3 BezierCurve::GetTangent(float t)
{
	float t2 = t * t;
	float tt1 = 1.0f - t;
	float tt2 = tt1 * tt1;
	glm::vec3 tangent = (-3.0f * tt2 * p0) + ((3.0f * tt2 - 6.0f * t * tt1) * p1) + ((6.0f * t * tt1 - 3.0f * t2) * p2) + (3.0f * t2 * p3);
	return glm::normalize(tangent);
}