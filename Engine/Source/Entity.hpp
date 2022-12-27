#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <cassert>
#include <unordered_map>
#include <queue>

#include "Components/Component.hpp"

class Entity
{
private:
    /**
     * @brief An unordered map as the component pool
     */
    std::unordered_map<std::string, std::shared_ptr<Component>> boundComponents{};
    /**
     * @brief A queue for components which OnCreate function hasn't been called yet
     */
    std::queue<std::shared_ptr<Component>> uncreatedComponents{};

    unsigned long long m_id;
    
public:
    /**
     * @brief Create a new object of type entity
     */
    Entity(unsigned long long id) : m_id(id) {};
    virtual ~Entity() {
        printf("[EntityManager] Destroyed Entity with id %lld\n", m_id);
    }

    /**
     * @brief Gets called once whenever the program is initializing itself
     */
    void OnEntityCreate();

    /**
     * @brief Updates the components once per frame
     */
    void OnUpdateComponents();

    /**
     * @brief Updates the UI components once per frame
     */
    void OnUpdateUIComponents();

    /**
     * @brief Gets called when an entity is destroyed
     * 
     * @param checkDestroyOnReload Whether to check the DestroyOnReload() function
     */
    void OnEntityDestroy();

    /**
     * @brief Bind the component parameter as a component of an entity
     * 
     * @param component Gets the component we want
     */
    void AddComponent(std::shared_ptr<Component> component);

    void RemoveComponent(std::string componentName);

    /**
     * @brief Gets the wanted component
     * 
     * @param componentName Finds the wanted component by its name
     * 
     * @return Returns a weak_ptr to the wanted component
     */
    std::shared_ptr<Component> GetComponent(std::string componentName);

    std::string Tag {};

    inline unsigned long long GetID() { return m_id; }

    bool DestroyOnReload = true;
};