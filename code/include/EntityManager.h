#pragma once

#include <memory>
#include <unordered_set>

#include "Component.h"
#include "Entity.h"
#include "Event.h"
#include "SlotMap.h"

namespace nxt::core {

class EntityCreatedEvent : public Event<EntityCreatedEvent> {
public:
    EntityCreatedEvent(const Entity& entity)
        : created_entity_(entity) {}

    const Entity& GetCreatedEntity() const noexcept {
        return created_entity_;
    }

private:
    Entity created_entity_;
};

/**
 * @brief Manager class which controls the lifecycle of Entities in an Application.
 *        Current responsibility of this Manager includes:
 *        1. Creating new Entity
 *        2. Marking entities inactive or active
 *        3. Destroying exisiting Entity
 *        4. Querying validity of an Entity
 *        Currently an EntityManager can store max of 2^32 entities
 *
 */
class EntityManager : public EventSource {
public:
    static EntityManager* get();

    /**
     * @brief Create and return a new Entity
     *
     * @return Newly created entity
     */
    Entity createEntity(bool temporary = false) noexcept;

    /**
     * @brief Checks if the given Entity is still alive
     *
     * @param entity Entity to be checked
     * @return True if the passed Entity is still alive or false otherwise
     */
    bool isAlive(const Entity& entity) const noexcept;

    /**
     * @brief
     *
     * @param entity
     * @return true
     * @return false
     */
    bool isActive(const Entity& entity) const noexcept;

    /**
     * @brief Set the Active object
     *
     * @param entity
     * @param active
     */
    void setActive(const Entity& entity, bool active);

    /**
     * @brief
     *
     * @param entity
     * @return true
     * @return false
     */
    bool isTemporary(const Entity& entity) const noexcept;

    /**
     * @brief Set the Active object
     *
     * @param entity
     * @param active
     */
    void setTemporary(const Entity& entity, bool value);

    /**
     * @brief
     *
     * @param entity
     * @return true
     * @return false
     */
    bool destroyEntity(const Entity& entity) noexcept;

    template<class T>
    T* getComponent() const noexcept {
        static_assert(std::is_base_of<core::Component, T>::value,
                      "Can only get derived class of ess::core::Component class");
        assert(ess::core::ComponentLookup<T>::component_id != -1 &&
               "Component is not registered. Use RegisterComponent() first before using GetComponent()");
        return static_cast<T*>(components_[ess::core::ComponentLookup<T>::component_id].get());
    }

    template<class T>
    void registerComponent() {
        static_assert(std::is_base_of<core::Component, T>::value,
                      "Can only register derived class of ess::core::Component class");
        T* component = new T;
        components_.emplaceBack(component);
        component_map_.emplace(component->getName(), component);
        component->onRegister();
        ess::core::ComponentLookup<T>::component_id = components_.Size() - 1;
    }

    template<class T>
    void isComponentRegistered() const noexcept {
        static_assert(std::is_base_of<core::Component, T>::value,
                      "Can only query derived class of ess::core::Component class");
        return ess::core::ComponentLookup<T>::component_id != -1;
    }

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    Component* getComponent(const std::string& name) const noexcept {
        auto iter = component_map_.find(name);
        if (iter != component_map_.end()) {
            return iter->second;
        }
        return nullptr;
    }

    const SlotMap<Entity>& getEntities() const noexcept {
        return entity_storage_;
    }

    void clearAllEntities();

private:
    EntityManager() = default;
    /**
     * @brief
     *
     */
    struct EntityInfo {
        unsigned int index;
        unsigned int generation : 30;
        unsigned int temporary : 1;
        unsigned int active : 1;
        EntityInfo()
            : index(0)
            , generation(0)
            , active(0)
            , temporary(0) {}
        EntityInfo(unsigned int index, unsigned int generation, bool active, bool temporary)
            : index(index)
            , generation(generation)
            , active(active)
            , temporary(temporary) {}
    };

    SlotMap<EntityInfo> entities_;
    SlotMap<Entity> entity_storage_;
    Vector<std::unique_ptr<Component>> components_;
    std::unordered_map<std::string, Component*> component_map_;
};
}  // namespace nxt::core
