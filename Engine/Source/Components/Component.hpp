#pragma once

#include <memory>
#include <cassert>
#include <string>

class Entity;

class Component
{
public:
    /**
     * @brief Define a new object of type component
     */
    Component() = default;

    virtual ~Component() = default;
    virtual const char* ComponentName() { return nullptr; }

    bool Active = true;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate() {}
    virtual void OnUI() {}

    /**
     * @brief Bind the component to an entity by pointer
     * 
     * @param object A pointer to the object to which to bind the component
     */
    void BindToEntity(std::weak_ptr<Entity> object);

    /**
     * @brief Bind the component to an entity by id
     * 
     * @param id The id of the object to which to bind the component
     */
    void BindToEntity(unsigned long long id);

    /**
     * @brief Get the component of an entity by name
     */
    std::shared_ptr<Component> GetComponent(std::string);

protected:
    std::weak_ptr<Entity> boundEntity {};
};

/**
 * @brief Define a component of a type
 */
#define DEFINE_COMPONENT(TYPE) \
    TYPE() : Component() {}; \
    const char* ComponentName() override { return #TYPE; };

/**
 * @brief Define a derived component of type and the type of the base component
 */
#define DEFINE_DERIVED_COMPONENT(TYPE, BASETYPE) \
    TYPE() : BASETYPE() {}; \
    const char* ComponentName() override { return #TYPE; };

/**
 * @brief Get component by type
 */
#define GET_COMPONENT(ComponentType) std::static_pointer_cast<ComponentType>(GetComponent(#ComponentType))
#define GET_COMPONENT_FROM(From, ComponentType) std::static_pointer_cast<ComponentType>(From->GetComponent(#ComponentType))