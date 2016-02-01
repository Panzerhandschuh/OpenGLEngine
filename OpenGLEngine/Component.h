#ifndef COMPONENT_H
#define COMPONENT_H

#include "Shader.h"

class Entity;
class Transform;

class Component
{
public:
	Entity* entity;
	Transform* transform;

	virtual ~Component() { }
	virtual void Start() { }
	virtual void Update(GLfloat deltaTime) { }
	virtual void Draw(Shader& shader) { }
};

#endif