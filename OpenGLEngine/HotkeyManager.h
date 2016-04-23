#ifndef HOTKEYMANAGER_H
#define HOTKEYMANAGER_H

#include "Component.h"
#include "InputManager.h"
#include "Camera.h"
#include "Entity.h"
#include "PathFollower.h"

class HotkeyManager : public Component
{
public:
	void Update(GLfloat deltaTime);
};

#endif