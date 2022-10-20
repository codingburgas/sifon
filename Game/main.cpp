#include "raylib.h"

#include "./Managers/ECSManager.hpp"
#include "./Managers/EntityManager.hpp"
#include "./Managers/AppManager.hpp"
#include "./Components/SpriteComponent.hpp"
#include "./Components/TransformComponent2D.hpp"

#include <memory>
#include <iostream>

int main()
{	
	AppManager* appManager = AppManager::GetInstance();

	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	appManager->ChangeScene("res/scenes/first_scene.json");
	appManager->Initialize();

	SetExitKey(0);

	while (!appManager->ShouldQuit())
	{
		appManager->Update();
	}

	appManager->Deinitialize();

	return 0;
}