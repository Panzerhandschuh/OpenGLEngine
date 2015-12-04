#include "Entity.h"

Entity::~Entity()
{
	for (int i = 0; i < components.size(); i++)
		delete components[i];
	components.clear();
}

void Entity::Update()
{
	for (int i = 0; i < components.size(); i++)
		components[i]->Update();
}

void Entity::Draw()
{
	for (int i = 0; i < components.size(); i++)
		components[i]->Draw();
}