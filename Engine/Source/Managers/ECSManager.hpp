#pragma once

#include <cassert>
#include <memory>
#include "../Entity.hpp"
#include "../Components/Component.hpp"
#include "EntityManager.hpp"

namespace ECS
{
    /**
     * @brief Creates a new entity and returns it
     */
    static std::shared_ptr<Entity> CreateEntity()
    {
        return EntityManager::GetInstance()->CreateEntity();
    }

    /**
     * @brief Creates a new component of the given type and returns it
     */
    template <typename CT>
    static std::shared_ptr<CT> CreateComponent()
    {
        static_assert(std::is_base_of<Component, CT>::value, "CT must inherit from Component");
        return std::make_shared<CT>();
    }
}