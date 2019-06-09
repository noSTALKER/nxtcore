#include "../include/ECS/EntityManager.h"
#include "../include/ECS/Component.h"
#include "../include/ECS/System.h"

namespace nxt::core {

EntityManager*
EntityManager::get() {
    static EntityManager manager;
    return &manager;
}

Entity
EntityManager::createEntity(bool temporary) noexcept {
    // insert a invalid entity to get its key
    Entity entity;
    auto key = entities_.emplace();

    // update the inserted entity info with the values
    // returned as the key
    auto& entity_info = entities_.at(key);
    entity_info.generation = key.generation;
    entity_info.index = static_cast<unsigned int>(key.index);
    entity_info.active = 1;
    entity_info.temporary = temporary;

    entity_storage_.insert({static_cast<unsigned int>(key.index), key.generation});
    entity.index_ = static_cast<unsigned int>(key.index);
    entity.generation_ = key.generation;

    EntityCreatedEvent entity_event(entity);
    Emit(entity_event);

    return entity;
}

bool
EntityManager::isAlive(const Entity& entity) const noexcept {
    Key key{entity.index_, entity.generation_};
    return entities_.exist(key);
}

bool
EntityManager::isActive(const Entity& entity) const noexcept {
    Key key{entity.index_, entity.generation_};
    if (entities_.exist(key)) {
        return entities_.at(key).active;
    }

    return false;
}

void
EntityManager::setActive(const Entity& entity, bool active) {
    Key key{entity.index_, entity.generation_};
    if (entities_.exist(key)) {
        entities_.at(key).active = active;
    }
}

bool
EntityManager::isTemporary(const Entity& entity) const noexcept {
    Key key{entity.index_, entity.generation_};
    if (entities_.exist(key)) {
        return entities_.at(key).temporary;
    }

    return false;
}

void
EntityManager::setTemporary(const Entity& entity, bool value) {
    Key key{entity.index_, entity.generation_};
    if (entities_.exist(key)) {
        entities_.at(key).temporary = value;
    }
}

bool
EntityManager::destroyEntity(const Entity& entity) noexcept {
    Key key{entity.index_, entity.generation_};
    entity_storage_.erase(key);

    // delete the entity from all the components if present
    for (const auto& component : components_) {
        component->removeEntity(entity);
    }

    return entities_.erase(key);
}

void
EntityManager::clearAllEntities() {
    Vector<Entity> entities;
    entities.reserve(entity_storage_.size());
    for (auto entity : entity_storage_) {
        entities.pushBack(entity);
    }

    for (auto entity : entities) {
        destroyEntity(entity);
    }
}

}  // namespace nxt::core
