#pragma once

#include "Entity.h"
#include "Key.h"

namespace nxt::core {
/**
 * @brief Helper class create unique component_id for each component class
 *
 */
template<class T>
struct ComponentLookup {};

/**
 * @brief Abstract base class for all Components
 *
 */
class Component {
public:
    virtual const std::string& getName() const noexcept = 0;
    /**
     * @brief Add the entity to the component
     *
     * @param entity Entity to be added to the component
     * @return Key of the added entity in the component
     */
    virtual Key addEntity(const Entity& entity) noexcept = 0;

    /**
     * @brief Check if the given entity is present in the Component
     *
     * @param entity
     * @return true
     * @return false
     */
    virtual bool hasEntity(const Entity& entity) const noexcept = 0;

    /**
     * @brief Remove the given entity from the Component if it is present
     *
     * @param entity
     * @return true
     * @return false
     */
    virtual bool removeEntity(const Entity& entity) noexcept = 0;

    /**
     * @brief Check if the given key points to a valid entity in the Component
     *
     * @param key
     * @return true
     * @return false
     */
    virtual bool isValid(const Key& key) const noexcept = 0;

    /**
     * @brief Get the Key corresponding to the given Entity
     *
     * @param entity
     * @return K
     */
    virtual Key getKey(const core::Entity& entity) const noexcept = 0;

    virtual void onRegister() = 0;

    virtual ~Component() = default;
};
}  // namespace nxt::core

/**
 * @brief Use this macro to register a new component which needs exporting
 *
 */
#define NXTCORE_REGISTER_COMPONENT(Export, Component) \
    namespace nxt::core {                             \
    template<>                                        \
    struct Export ComponentLookup<Component> {        \
        static size_t component_id;                   \
    };                                                \
    }

/**
 * @brief Use this macro to register a new component which doesn't needs exporting
 *
 */
#define NXTCORE_REGISTER_COMPONENT_NO_EXPORT(Component) \
    namespace nxt::core {                               \
    template<>                                          \
    struct ComponentLookup<Component> {                 \
        static size_t component_id;                     \
    };                                                  \
    }

/**
 * @brief Use this macro to define a already registered component
 *
 */
#define NXTCORE_DEFINE_COMPONENT(Component)               \
    namespace nxt::core {                                 \
    size_t ComponentLookup<Component>::component_id = -1; \
    }
