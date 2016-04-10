#include "PathPoint.h"

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
