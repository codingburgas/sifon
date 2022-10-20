#include "AppManager.hpp"

/**
* @brief Creates a null pointer instance of the singleton class GameManager
*/
AppManager* AppManager::instance = nullptr;

AppManager* AppManager::GetInstance()
{
    /**
     * @brief If an instance of the singleton doesn't exist, creates one
     */
    if (instance == nullptr)
    {
        instance = new AppManager();
    }

    return instance;
}

void AppManager::ReinitializeScene()
{
    /**
     * @brief Destroys any leftover drawables to prevent memory leaking
     */
    EntityManager::GetInstance()->DestroyAllEntities();
    InitSceneFromFile(state.scenePath);
    EntityManager::GetInstance()->TriggerCreateEvents();
    
    /**
     * @brief After the scene is updated, stops it from updating again
     */
    state.scene_update = false;
}

void AppManager::Initialize()
{
    /**
     * @brief Initializes the window and imgui
     */
    if(!Graphics::InitWindow(Vec2f{ WINDOW_WIDTH, WINDOW_HEIGHT }, WINDOW_TITLE))
        exit(1);

    ReinitializeScene();
}

void AppManager::Quit()
{
    /**
     * @brief Makes the game quit
     */
    std::cout << "INFO: GameManager: Triggered should_quit boolean" << std::endl;
    state.should_quit = true;
}

void AppManager::Update()
{
    /**
     * @brief If a scene has been changed, updates it
     */
    if (state.scene_update)
    {
        ReinitializeScene();
        std::cout << "INFO: GameManager: Scene Update!" << std::endl;
    }

    if (IsKeyDown(KEY_R))
        ReinitializeScene();

    Graphics::BeginDrawing();

    /**
     * @brief  Draws and updates all drawables
     */
    EntityManager::GetInstance()->TriggerUpdateEvents();

    Graphics::EndDrawing();
}

void AppManager::Deinitialize()
{
    /**
     * @brief Destroys any leftover drawables to prevent memory leaking
     */

    EntityManager::GetInstance()->DestroyAllEntities(true);

    Graphics::CloseWindow();

    std::cout << "INFO: Goodbye!" << std::endl;
}

bool AppManager::ShouldQuit()
{
    /**
     * @brief Returns if the window should quit
     */
    return state.should_quit;
}

void AppManager::ChangeScene(std::string scenePath)
{
    /**
     * @brief Sets the path for the scene and makes it update
     * 
     * @param scenePath Gets the path of the .json file for the scene
     */
    state.scenePath = scenePath;
    state.scene_update = true;
}