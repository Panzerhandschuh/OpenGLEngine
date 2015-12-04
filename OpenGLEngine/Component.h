#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
	Entity* entity;

	virtual ~Component() { }
	virtual void Update() { }
	virtual void Draw() { }
};

#endif