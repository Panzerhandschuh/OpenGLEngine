#include "Entity.h"

Entity::~Entity()
{
	for (int i = 0; i < components.size(); i++)
		delete components[i];
	components.clear();
}

void Entity::Update(GLfloat deltaTime)
{
	for (int i = 0; i < components.size(); i++)
		components[i]->Update(deltaTime);
}

void Entity::Draw(Shader& shader)
{
	for (int i = 0; i < components.size(); i++)
		components[i]->Draw(shader);
}