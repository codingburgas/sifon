#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <type_traits>

#include "../config.h"

#include "EntityManager.hpp"
#include "../Entity.hpp"

#include "../SceneReader.hpp"
#include "../Util/graphics.hpp"

class AppManager
{
private:
    struct GameState
    {
        bool scene_update;
        bool should_quit;
        std::string scenePath;
    } state;

    /**
     * @brief Reinitializes the current scene
     * 
     * @note Meant to be used in the Update() function
     */
    void ReinitializeScene();

    static AppManager* instance;

private:
    AppManager() : state { false, false, {} } {};
    ~AppManager() = default;

public:
    /**
     * @brief Diallows cloning of the singleton
     */
    AppManager(AppManager const&) = delete;

    /**
     * @brief Disallows assigning of the singleton
     */
    void operator=(AppManager const&) = delete;

    /**
     * @brief Gets the instance of the singleton class GameManager
     */
    static AppManager* GetInstance();

    /**
     * @brief Initializes the program
     * 
     * @note Meant to be used in main.cpp
     */
    void Initialize();

    /**
     * @brief Deinitializes the program
     * 
     * @note Meant to be used in main.cpp
     */
    void Deinitialize();

    /**
     * @brief Steps or updates the program
     * 
     * @note Meant to be used in main.cpp
     */
    void Update();

    /**
     * @brief Similar to the WindowShouldClose() function in raylib.h
     * 
     * @note Meant to be used in main.cpp
     * 
     * @return bool
     */
    bool ShouldQuit();

    /**
     * @brief An API function, which triggers a boolean telling the program it should quit;
     * 
     * @attention Meant to be used at any point in the code (unlike functions like initialize and deinitialize)
     */
    void Quit();

    /**
     * @brief An API function, which changes the current scene
     * 
     * @param scene This describes the current scene
     * 
     * @note Meant to be used at any point in the code
     */
    void ChangeScene(std::string scenePath);
};