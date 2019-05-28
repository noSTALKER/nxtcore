#pragma once

#include <limits>
#include <string>
#include <unordered_map>

namespace nxt::core {

class Entity {
public:
    //! ID of an invalid entity. In case a functions needs to return a Invalid Entity,
    //! use this Entity always
    static const Entity kInvalidEntity;

    Entity() noexcept = default;

    /**
     * @brief Construct a new Entity instance. Use this constructor to create special static entity only.
     *        Use EntityManager to create new Entity normally.
     *
     * @param index Index of the Entity
     * @param generation Current Entity Generation
     */
    constexpr Entity(uint32_t index, uint32_t generation) noexcept
        : index_(index)
        , generation_(generation) {}

    /**
     * @brief Copy Constructor
     *
     */
    Entity(const Entity&) = default;
    /**
     * @brief Assignment Operator
     *
     * @return instance reference after assignment
     */
    Entity& operator=(const Entity&) = default;

    /**
     * @brief Move Constructor
     *
     */
    Entity(Entity&&) = default;

    /**
     * @brief Move Assignment operator
     *
     * @return instance reference after assignment
     */
    Entity& operator=(Entity&&) = default;

    /**
     * @brief
     *
     * @param rhs
     * @return true
     * @return false
     */
    [[nodiscard]] constexpr bool operator==(const Entity& rhs) const noexcept {
        return index_ == rhs.index_ && generation_ == rhs.generation_;
    }

    [[nodiscard]] constexpr bool operator!=(const Entity& rhs) const noexcept {
        return index_ != rhs.index_ || generation_ != rhs.generation_;
    }

    [[nodiscard]] std::string getString() const noexcept {
        return std::to_string(index_) + '_' + std::to_string(generation_);
    }

    [[nodiscard]] constexpr bool isValid() const noexcept {
        return (*this != kInvalidEntity) ? true : false;
    }

    friend class EntityManager;
    friend struct std::hash<Entity>;

private:
    //! index of the entity. used in the entity manager
    uint32_t index_ = std::numeric_limits<uint32_t>::max();

    //! current entity generation
    uint32_t generation_ = 0;
};
}  // namespace core

namespace std {
//! Specialization of std::hash for ess::core::Entity so that it can be used
//! in std::unordered_map and std::unordered_set
template<>
struct hash<nxt::core::Entity> {
    size_t operator()(const nxt::core::Entity& entity) const noexcept {
        return entity.index_;
    }
};
}  // namespace std
