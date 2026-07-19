#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

/// @brief Updates position for all entities with Transform + Velocity.
/// Entities with PlayerTag are skipped (handled by PlayerMovementSystem).
class MovementSystem : public System {
public:
    /// @param cm Reference to the component manager.
    explicit MovementSystem(ComponentManager& cm);

    /// @brief Moves all non-player entities by velocity * dt.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;
};

#endif // MOVEMENT_SYSTEM_HPP