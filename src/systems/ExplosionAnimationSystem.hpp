#ifndef EXPLOSION_ANIMATION_SYSTEM_HPP
#define EXPLOSION_ANIMATION_SYSTEM_HPP

#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Entity.hpp"

/// @brief Animates explosion entities through spritesheet frames.
/// Updates the texture rect of each explosion sprite to show the current frame.
/// Removes the entity when the animation loop completes.
class ExplosionAnimationSystem : public System {
public:
    /// @param cm Reference to the component manager.
    explicit ExplosionAnimationSystem(ComponentManager& cm);

    /// @brief Advances frame timers and updates sprite texture rects.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;

    /// @brief Removes all components associated with an explosion entity.
    void remove_explosion_components(Entity entity);
};

#endif // EXPLOSION_ANIMATION_SYSTEM_HPP