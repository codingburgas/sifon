#include "Entity.hpp"

void Entity::OnEntityCreate()
{
    /**
     * @brief gets every component in the uncreatedComponents queue, creates it and puts it in the boundComponents table
     */
    while (!uncreatedComponents.empty())
    {
        auto component = uncreatedComponents.front();
        
        assert(boundComponents.find(component->ComponentName()) == boundComponents.end() && "An attempt was made to bind an already existing component type");
        
        uncreatedComponents.pop();
        boundComponents[component->ComponentName()] = component;
        component->OnCreate();
    }
    /*for (auto& [key, component] : boundComponents)
    {
        component->OnCreate();
    }*/
}

void Entity::OnUpdateComponents()
{
    /**
     * @brief Gets every component in the component bool and calls the OnUpdate() function if they're active
     */
    for (auto& [key, component] : boundComponents)
    {
        if (component->Active)
            component->OnUpdate();
    }
}

void Entity::OnUpdateUIComponents()
{
    /**
     * @brief Gets every component in the component bool and calls the OnUI() function if they're active
     */
    for (auto& [key, component] : boundComponents)
    {
        if (component->Active)
            component->OnUI();
    }
}

void Entity::OnEntityDestroy()
{
    /**
     * @brief Gets every component in the component bool and calls the OnDestroy() function if it's destroyed
     */
    for (auto& [key, component] : boundComponents)
    {
        component->OnDestroy();
    }
}

void Entity::AddComponent(std::shared_ptr<Component> component)
{
    assert(component->ComponentName() != nullptr && "An attempt was made to bind an instance of the base Component class");
    assert((component.use_count() - 1) <= 1 && "An attempt was made to bind a used Component");

    /**
     * @brief Sets the component name and binds it to the entity in which it's used, then adds it to the component pool
     */
    std::string componentName = component->ComponentName();
    assert(boundComponents.find(componentName) == boundComponents.end() && "An attempt was made to bind an already existing component type");
    
    component->BindToEntity(this->GetID());
    uncreatedComponents.push(component);
    //boundComponents[componentName] = component;
}

void Entity::RemoveComponent(std::string componentName)
{
    if (boundComponents.find(componentName) == boundComponents.end()) return;

    boundComponents.at(componentName)->OnDestroy();
    boundComponents.erase(componentName);
}

std::shared_ptr<Component> Entity::GetComponent(std::string componentName)
{
    /**
     * @brief If the component is found in the pool, it returns it, else it returns empty
     */
    if (boundComponents.find(componentName) != boundComponents.end())
        return boundComponents[componentName];
    else return std::shared_ptr<Component>();
}