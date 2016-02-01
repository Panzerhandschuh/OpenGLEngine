#include "EntityManager.h"

std::vector<Entity*> EntityManager::entities;

Entity* EntityManager::CreateEntity()
{
	Entity* ent = new Entity;
	Transform* transform = ent->AddComponent<Transform>();
	ent->transform = transform;
	entities.push_back(ent);
	return ent;
}

void EntityManager::DestroyEntity(Entity* ent)
{
	std::vector<Entity*>::iterator pos = std::find(entities.begin(), entities.end(), ent);
	if (pos != entities.end())
	{
		delete ent;
		entities.erase(pos);
	}
}

void EntityManager::UpdateAll(GLfloat deltaTime)
{
	for (int i = 0; i < entities.size(); i++)
		entities[i]->Update(deltaTime);
}

void EntityManager::DrawAll(Shader& shader)
{
	for (int i = 0; i < entities.size(); i++)
		entities[i]->Draw(shader);
}