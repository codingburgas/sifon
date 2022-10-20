#include "../Managers/EntityManager.hpp"
#include "../Components/Component.hpp"
#include "../Entity.hpp"

void Component::BindToEntity(std::weak_ptr<Entity> object)
{
    /**
     * @brief Binds the component to an entity
     */
    boundEntity = object;
}

void Component::BindToEntity(unsigned long long id)
{
    /**
     * @brief Gets the entity by id and binds the component to it
     */
    auto object = EntityManager::GetInstance()->GetEntityFromID(id);
    BindToEntity(object);
}

std::shared_ptr<Component> Component::GetComponent(std::string componentName)
{
    /**
     * @brief Gets the current entity and returns the component with the desired name
     */
    auto entity = boundEntity.lock();
    return entity->GetComponent(componentName);
}