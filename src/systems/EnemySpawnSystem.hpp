#ifndef ENEMY_SPAWN_SYSTEM_HPP
#define ENEMY_SPAWN_SYSTEM_HPP

#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Entity.hpp"

/// @brief Periodically spawns enemy entities above the top of the screen.
/// Enemies move downward at a constant speed.
class EnemySpawnSystem : public System {
public:
    /// @param cm            Reference to the component manager.
    /// @param screenWidth   Screen width in pixels (for random X placement).
    /// @param screenHeight  Screen height in pixels (for spawning above).
    /// @param nextId        Pointer to the shared entity ID counter (owned by Game).
    /// @param interval      Time in seconds between spawns.
    /// @param speed         Enemy downward speed in pixels per second.
    /// @param texture       Shared texture for enemy sprite (nullptr = fallback to Shape).
    EnemySpawnSystem(ComponentManager& cm,
                     float screenWidth,
                     float screenHeight,
                     EntityId* nextId,
                     float interval = 2.0f,
                     float speed = 150.0f,
                     std::shared_ptr<sf::Texture> texture = nullptr);

    /// @brief Updates spawn timer and creates enemies as needed.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;
    float screen_width_;
    float screen_height_;
    float spawn_interval_;
    float enemy_speed_;
    float spawn_timer_ = 0.0f;

    /// @brief Pointer to the shared entity ID counter (owned by Game).
    EntityId* next_entity_id_;

    std::shared_ptr<sf::Texture> enemy_texture_;  ///< Shared texture for enemy sprites (may be null).

    std::mt19937 rng_;
    std::uniform_real_distribution<float> x_dist_;

    /// @brief Spawns a single enemy at a random X above the screen.
    void spawn_enemy();
};

#endif // ENEMY_SPAWN_SYSTEM_HPP