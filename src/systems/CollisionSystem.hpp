#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
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
    /// @param score Optional pointer to an external score counter (may be null).
    /// @param hitSound Pointer to the sf::Sound for bullet-enemy hit (may be null).
    /// @param explosionSound Pointer to the sf::Sound for enemy destruction (may be null).
    /// @param nextId Pointer to the shared entity ID counter (owned by Game).
    /// @param explosionTexture Shared texture for explosion spritesheet (may be null → no animation).
    CollisionSystem(ComponentManager& cm,
                    int* score = nullptr,
                    sf::Sound* hitSound = nullptr,
                    sf::Sound* explosionSound = nullptr,
                    EntityId* nextId = nullptr,
                    std::shared_ptr<sf::Texture> explosionTexture = nullptr);

    /// @brief Checks all bullet-enemy pairs for AABB overlap and resolves collisions.
    /// @param dt Delta time in seconds (unused — collisions are frame-independent).
    void update(float dt) override;

private:
    ComponentManager& cm_;
    int* score_;
    sf::Sound* hit_sound_;
    sf::Sound* explosion_sound_;
    EntityId* next_entity_id_;
    std::shared_ptr<sf::Texture> explosion_texture_;

    /// @brief Removes all components associated with a bullet entity.
    /// @param entity The bullet entity to clean up.
    void remove_bullet_components(Entity entity);

    /// @brief Removes all components associated with an enemy entity.
    /// @param entity The enemy entity to clean up.
    void remove_enemy_components(Entity entity);

    /// @brief Spawns an explosion animation entity at the given position.
    /// @param position World position where the explosion occurs.
    void spawn_explosion(const sf::Vector2f& position);
};

#endif // COLLISION_SYSTEM_HPP