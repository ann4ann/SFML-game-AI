#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>

/// @brief Unique identifier for an entity.
using EntityId = std::uint32_t;

/// @brief Invalid entity ID constant.
constexpr EntityId INVALID_ENTITY = 0;

/// @brief Represents a game entity — just an ID.
/// Components are stored externally in a ComponentManager.
struct Entity {
    EntityId id = INVALID_ENTITY;

    explicit Entity(EntityId id_) noexcept : id(id_) {}

    bool operator==(const Entity& other) const noexcept { return id == other.id; }
    bool operator!=(const Entity& other) const noexcept { return id != other.id; }
    bool operator<(const Entity& other) const noexcept { return id < other.id; }
};

#endif // ENTITY_HPP