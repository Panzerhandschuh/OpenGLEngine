#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "Entity.h"

class EntityManager
{
public:
	static Entity* CreateEntity();
	static void DestroyEntity(Entity* ent);
	static void UpdateAll();
	static void DrawAll();

private:
	static std::vector<Entity*> entities;
};

#endif