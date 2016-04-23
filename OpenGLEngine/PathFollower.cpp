#include "PathFollower.h"

using namespace glm;

void PathFollower::Update(GLfloat deltaTime)
{
	BezierCurve curve = currentPoint->GetCurve();
	float curveLength = curve.GetLength();

	// Find acceleration
	float t = currentPos / curveLength;
	float tLength = curve.GetLengthParameter(t);
	vec3 tang = curve.Derivative(tLength);
	float accel = dot(tang, vec3(0.0f, -1.0f, 0.0f));

	// Update velocity and position
	velocity += accel * deltaTime;
	if (velocity < minSpeed)
		velocity = minSpeed;
	currentPos += velocity * deltaTime;

	if (currentPos > curveLength)
	{
		currentPos -= curveLength;

		if (currentPoint->next->IsEnd())
		{
			// Reset follower
			velocity = minSpeed;
			currentPoint = startPoint;
		}
		else
			currentPoint = currentPoint->next;
		curve = currentPoint->GetCurve();
		curveLength = curve.GetLength();
	}

	t = currentPos / curveLength;
	tLength = curve.GetLengthParameter(t);
	transform->position = currentPoint->GetPoint(curve, tLength) + offset;
	transform->rotation = quat(currentPoint->GetRotation(curve, tLength));
}

void PathFollower::Init(PathPoint* point, vec3 offset)
{
	startPoint = point;
	currentPoint = point;
	this->offset = offset;
	velocity = minSpeed;
}

void PathFollower::StartFollow()
{
	currentPoint = startPoint;
	currentPos = 0.0f;
	velocity = minSpeed;
}
