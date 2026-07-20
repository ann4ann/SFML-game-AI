#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <vector>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Entity.hpp"

/// @brief Detects collisions between bullets and enemies using AABB intersection (SFML FloatRect).
/// Bullets are always destroyed on collision; enemies take damage (Health -1) and are
/// destroyed if their HP reaches 0. Uses deferred removal to avoid iterator invalidation.
class CollisionSystem : public System {
public:
    /// @param cm Reference to the component manager.
    explicit CollisionSystem(ComponentManager& cm);

    /// @brief Checks all bullet-enemy pairs for AABB overlap and resolves collisions.
    /// @param dt Delta time in seconds (unused — collisions are frame-independent).
    void update(float dt) override;

private:
    ComponentManager& cm_;

    /// @brief Removes all components associated with a bullet entity.
    /// @param entity The bullet entity to clean up.
    void remove_bullet_components(Entity entity);

    /// @brief Removes all components associated with an enemy entity.
    /// @param entity The enemy entity to clean up.
    void remove_enemy_components(Entity entity);
};

#endif // COLLISION_SYSTEM_HPP