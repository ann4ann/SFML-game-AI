#ifndef BULLET_CLEANUP_SYSTEM_HPP
#define BULLET_CLEANUP_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

/// @brief Removes bullets that have left the screen or exceeded their lifetime.
class BulletCleanupSystem : public System {
public:
    /// @param cm           Reference to the component manager.
    /// @param screenHeight Screen height in pixels (bullets removed above top).
    BulletCleanupSystem(ComponentManager& cm, float screenHeight);

    /// @brief Checks all bullets and removes those that are off-screen or expired.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;
    float screen_height_;

    /// @brief Removes all bullet-specific components from an entity.
    /// @param entity The bullet entity to clean up.
    void remove_bullet_components(Entity entity);
};

#endif // BULLET_CLEANUP_SYSTEM_HPP