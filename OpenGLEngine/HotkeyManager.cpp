#include "HotkeyManager.h"

void HotkeyManager::Update(GLfloat deltaTime)
{
	if (InputManager::GetKeyDown(GLFW_KEY_P))
	{
		//PathFollower* follower = Camera::main->entity->AddComponent<PathFollower>();
		//follower->Init()
	}
	else if (InputManager::GetKeyDown(GLFW_KEY_ESCAPE))
	{

	}
}
