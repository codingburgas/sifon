#include "EntityManager.hpp"
#include "../Util/graphics.hpp"
#include "../Entity.hpp"
#include "../Components/Component.hpp"

/**
 * @brief The current id for the latest object in the object pool
 */
unsigned long long EntityManager::currentId = 0;

/**
 * @brief Creates a null pointer instance of the singleton class EntityManager
 */
EntityManager* EntityManager::instance = nullptr;

EntityManager* EntityManager::GetInstance()
{
    /**
     * @brief If an instance of the singleton doesn't exist, creates one
     */
    if (instance == nullptr)
        instance = new EntityManager();

    return instance;
}

size_t EntityManager::GetEntityCount()
{
    /**
     * @brief Gets the size of the object pool map and returns it
     */
    return entityTable.size();
}

std::shared_ptr<Entity> EntityManager::GetEntityFromID(unsigned long long id) {
    if (entityTable.find(id) != entityTable.end())
        return entityTable[id];
    else return std::shared_ptr<Entity>(); /* null */
}

void EntityManager::DestroyAllEntities(bool totalDevastation)
{
    /**
     * @brief Creates a list with ids to destroy, checks if they are entities and if they are adds them to it and then calls the OnEntityDestroy() function
     */
    std::list<unsigned long long> idsForDestroying;

    for (auto& [key, entity] : entityTable)
    {
        if (entity->DestroyOnReload || totalDevastation)
            idsForDestroying.push_back(entity->GetID());
    }

    /**
     * @brief Destroys every entity in the idsForDestroying list
     */
    for (auto id : idsForDestroying)
        DestroyEntityFromID(id);
}

void EntityManager::DestroyEntityFromID(unsigned long long id)
{
    /**
     * @brief Goes through the object pool and if it finds the id we are looking for, destroys it
     */
    if (entityTable.find(id) != entityTable.end())
    {
        std::static_pointer_cast<Entity>(entityTable[id])->OnEntityDestroy();
        entityTable.erase(id);
    }
}

std::shared_ptr<Entity> EntityManager::GetEntityFromTagName(std::string name)
{
    /**
     * @brief Goes through the object pool and if it finds the entity with the tag name we are looking for, returns it. If not, returns empty
     */
    for (auto& [key, entity] : entityTable)
    {
        if (entity->Tag == name)
            return entity;
    }

    return std::shared_ptr<Entity>();
}

bool EntityManager::TagNameAlreadyExists(std::string name)
{
    /**
     * @brief Goes through the object pool and if it finds an entity with the tag name param, returns true. If not, returns false
     */
    for (auto& [key, entity] : entityTable)
    {
        if (entity->Tag == name)
            return true;
    }

    return false;
}

void EntityManager::TriggerCreateEvents()
{
    /**
     * @brief Goes through the object pool and if it finds an entity, calls the OnEntityCreate() function for it
     */
    for (auto& [key, entity] : entityTable)
    {
        entity->OnEntityCreate();
    }
}

void EntityManager::TriggerUpdateEvents()
{

    /**
     * @brief Goes through the object pool and if it finds an entity, calls the OnUpdateComponents() function for it
     */
    for (auto& [key, entity] : entityTable)
    {
        entity->OnUpdateComponents();
    }
}