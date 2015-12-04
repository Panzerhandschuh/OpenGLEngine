#include "EntityManager.h"

std::vector<Entity*> EntityManager::entities;

Entity* EntityManager::CreateEntity()
{
	Entity* ent = new Entity;
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

void EntityManager::UpdateAll()
{
	for (int i = 0; i < entities.size(); i++)
		entities[i]->Update();
}

void EntityManager::DrawAll()
{
	for (int i = 0; i < entities.size(); i++)
		entities[i]->Draw();
}