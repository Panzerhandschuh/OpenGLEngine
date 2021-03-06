#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#define NUM_STEPS 20

class BezierCurve
{
public:
	glm::vec3 p0, p1, p2, p3;
	std::vector<float> arcLengths;

	BezierCurve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	void SetControlPoints(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 GetPoint(float t);
	glm::vec3 Derivative(float t);
	glm::vec3 Derivative2(float t);
	float GetCurvature(float t);
	float GetMaxCurvature();
	float GetLength();
	float GetLengthParameter(float t);

private:
	

	void CalcLength();
};

#endif