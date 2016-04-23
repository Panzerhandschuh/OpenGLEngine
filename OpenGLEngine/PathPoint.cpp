#include "PathPoint.h"

using namespace glm;

void PathPoint::Start()
{
	pointModel = new Model("Sphere.FBX");

	SphereCollider* collider = entity->AddComponent<SphereCollider>();
	collider->radius = 1.0f;

	// Create start and end bezier handles
	Entity* start = EntityManager::CreateEntity();
	PathPointHandle* startHandle = start->AddComponent<PathPointHandle>();
	this->startHandle = start->transform;

	Entity* end = EntityManager::CreateEntity();
	PathPointHandle* endHandle = end->AddComponent<PathPointHandle>();
	this->endHandle = end->transform;

	startHandle->otherHandle = this->endHandle;
	endHandle->otherHandle = this->startHandle;
}

PathPoint::~PathPoint()
{
	delete pointModel;
}

void PathPoint::Update(GLfloat deltaTime)
{
	bool isSelected = (SelectionManager::instance->selectedEnt == entity);
	startHandle->entity->enabled = isSelected;
	endHandle->entity->enabled = isSelected;
}

void PathPoint::Draw(Shader & shader)
{
	shader.Use();

	// Draw point
	bool isSelected = (SelectionManager::instance->selectedEnt == entity);
	vec3 color = (isSelected) ? vec3(0.0f, 0.5f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);
	shader.SetUniform("objectColor", color);
	mat4 model;
	model = translate(mat4(1.0f), transform->position);
	model = scale(model, vec3(2.0f));
	shader.SetUniform("model", model);
	pointModel->Draw();

	if (isSelected)
	{
		// Draw tangent points
		bool isStartSelected = (SelectionManager::instance->selectedHandle == startHandle->entity);
		vec3 startcolor = (isStartSelected) ? vec3(1.0f, 1.0f, 0.0f) : vec3(1.0f, 1.0f, 1.0f);
		shader.SetUniform("objectColor", startcolor);
		model = translate(mat4(1.0f), startHandle->position);
		model = scale(model, vec3(1.0f));
		shader.SetUniform("model", model);
		pointModel->Draw();

		bool isEndSelected = (SelectionManager::instance->selectedHandle == endHandle->entity);
		vec3 endcolor = (isEndSelected) ? vec3(1.0f, 1.0f, 0.0f) : vec3(1.0f, 1.0f, 1.0f);
		shader.SetUniform("objectColor", endcolor);
		model = translate(mat4(1.0f), endHandle->position);
		shader.SetUniform("model", model);
		pointModel->Draw();

		// Draw tangent lines
		LineUtil::DrawLine(transform->position, startHandle->position);
		LineUtil::DrawLine(transform->position, endHandle->position);
	}
}

void PathPoint::Init(glm::vec3 pos, glm::vec3 dir)
{
	transform->position = pos;
	transform->rotation = normalize(quat(orientation(dir, vec3(0.0f, 0.0f, 1.0f))));
	startHandle->position = transform->position + (-5.0f * dir);
	endHandle->position = transform->position + (5.0f * dir);
}

void PathPoint::UpdateHandles(glm::vec3 moveDelta)
{
	startHandle->position += moveDelta;
	endHandle->position += moveDelta;
}

bool PathPoint::IsStart()
{
	return (!prev);
}

bool PathPoint::IsEnd()
{
	return (!next);
}

bool PathPoint::IsIntermediate()
{
	return (next && prev);
}

glm::vec3 PathPoint::GetDirection()
{
	return normalize(endHandle->position - startHandle->position);
}

glm::vec3 PathPoint::GetPoint(BezierCurve& curve, float t)
{
	return curve.GetPoint(t);
}

glm::mat4 PathPoint::GetRotation(BezierCurve& curve, float t)
{
	vec3 tang = normalize(curve.Derivative(t));
	quat startRot = angleAxis(angle, GetDirection());
	quat endRot = angleAxis(next->angle, next->GetDirection());
	quat sRot = slerp(startRot, endRot, smoothstep(0.0f, 1.0f, t));

	vec3 up = QuaternionUtil::GetUp(sRot);
	vec3 biNormal = normalize(cross(up, tang));
	vec3 norm = normalize(cross(tang, biNormal));

	return mat4(biNormal.x, biNormal.y, biNormal.z, 0.0f,
		norm.x, norm.y, norm.z, 0.0f,
		tang.x, tang.y, tang.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

BezierCurve PathPoint::GetCurve()
{
	return BezierCurve(transform->position, endHandle->position, next->startHandle->position, next->transform->position);
}
