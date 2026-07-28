#ifndef ZIGZAG_SYSTEM_HPP
#define ZIGZAG_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

/// @brief Updates horizontal velocity for zigzag enemies to create sine-wave movement.
/// Operates on entities that have Transform + Velocity + Zigzag components.
/// Sets velocity.x = amplitude * cos(frequency * elapsed * 2π) each frame.
class ZigzagSystem : public System {
public:
    /// @param cm Reference to the component manager.
    explicit ZigzagSystem(ComponentManager& cm);

    /// @brief Updates zigzag velocity for all matching entities.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;
};

#endif // ZIGZAG_SYSTEM_HPP