#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "Entity.h"
#include "Shader.h"

class EntityManager
{
public:
	static std::vector<Entity*> entities;

	static Entity* CreateEntity();
	static void DestroyEntity(Entity* ent);
	static void UpdateAll(GLfloat deltaTime);
	static void DrawAll(Shader& shader);

private:
	
};

#endif