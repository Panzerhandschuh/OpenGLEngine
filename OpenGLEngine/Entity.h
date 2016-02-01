#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Transform.h"
#include "Component.h"

class Entity
{
public:
	Transform* transform;

	~Entity();
	void Update(GLfloat deltaTime);
	void Draw(Shader& shader);
	template<class T> T* AddComponent();
	template<class T> T* GetComponent();
	template<class T> void RemoveComponent();

private:
	std::vector<Component*> components;
};

template<class T> T* Entity::AddComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(components[i]);
		if (comp != 0)
		{
			std::cout << "Error: Could not add component, component already exists" << std::endl;
			return 0;
		}
	}

	T* comp = new T;
	comp->entity = this;
	comp->transform = transform;
	comp->Start();

	components.push_back(comp);

	return comp;
}

template<class T> T* Entity::GetComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(components[i]);
		if (comp != 0)
			return comp;
	}

	return 0;
}

template<class T> void Entity::RemoveComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		T* comp = dynamic_cast<T*>(components[i]);
		if (comp != 0)
		{
			delete comp;
			components.erase(components.begin() + i);
			return;
		}
	}

	std::cout << "Error: Could not remove component, component does not exist" << std::endl;
}

#endif