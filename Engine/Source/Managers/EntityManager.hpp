#pragma once

#include <list>
#include <unordered_map>
#include <map>
#include <memory>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <type_traits>
#include <vector>

#include "../Entity.hpp"

class EntityManager
{
private:
    /**
     * @brief An unordered map for the object pool
     */
    std::unordered_map<unsigned long long, std::shared_ptr<Entity>> entityTable {};

    static unsigned long long currentId;
    static EntityManager* instance;

    EntityManager() : entityTable() {};
    ~EntityManager() = default;

public:
    /**
     * @brief Diallows cloning of the singleton
     */
    EntityManager(EntityManager const&) = delete;

    /**
     * @brief Disallows assigning of the singleton
     */
    void operator=(EntityManager const&) = delete;

    /**
     * @brief Gets the instance of the singleton class EntityManager
     */
    static EntityManager* GetInstance();

    /**
     * @brief Creates an object with an id, adds it to the object pool, moves on to the next id and returns the object
     * 
     * @return Returns a shared_ptr of the newly-created object
     */
    std::shared_ptr<Entity> CreateEntity()
    {
        std::shared_ptr<Entity> current = std::make_shared<Entity>(currentId);
        entityTable[currentId] = current;
        printf("[EntityManager] Created Entity with id %lld\n", currentId);
        currentId++;
        return current;
    }

    /**
     * @brief Gets the amount of objects in the object pool
     */
    size_t GetEntityCount();

    /**
     * @brief Destroys all of the objects in the object pool
     * 
     * @param totalDevastation Used to destroy *all* components (even the ones with DestroyOnReload = false) 
     */
    void DestroyAllEntities(bool totalDevastation = false);

    /**
     * @brief Destroys an entity by id
     * 
     * @param id The id of the entity we want to destroy
     */
    void DestroyEntityFromID(unsigned long long id);

    std::shared_ptr<Entity> GetEntityFromID(unsigned long long id);

    /**
     * @brief Gets an object by tag name in the object pool and returns it
     * 
     * @param name The tag name of the object we need
     * 
     * @return Returns the object it gets from the tag name
     */
    std::shared_ptr<Entity> GetEntityFromTagName(std::string name);

    /**
     * @brief Returns if an object with a tag name already exists in the object pool
     * 
     * @param name The tag name which we need to find out if exists
     * 
     * @return Returns whether the tag name already exists
     */
    bool TagNameAlreadyExists(std::string name);

    /**
     * @brief Creates all drawables
     */
    void TriggerCreateEvents();

    /**
     * @brief Draws and updates all drawables
     */
    void TriggerUpdateEvents();
};
