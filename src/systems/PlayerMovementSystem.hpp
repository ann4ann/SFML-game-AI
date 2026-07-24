#ifndef PLAYER_MOVEMENT_SYSTEM_HPP
#define PLAYER_MOVEMENT_SYSTEM_HPP

#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

/// @brief Handles player input (movement + shooting) each frame.
/// Reads keyboard state, updates player Transform and Velocity accordingly,
/// clamps the player inside the screen bounds, and spawns bullets on Space.
class PlayerMovementSystem : public System {
public:
    /// @param cm            Reference to the component manager.
    /// @param screenWidth    Screen width in pixels (for clamping).
    /// @param screenHeight   Screen height in pixels (for clamping).
    /// @param nextId         Pointer to the shared entity ID counter.
    /// @param speed          Player movement speed in pixels per second.
    /// @param bulletSpeed    Bullet upward speed in pixels per second.
    /// @param cooldown       Minimum interval between shots in seconds.
    /// @param bulletSize     Side length of the square bullet in pixels.
    PlayerMovementSystem(ComponentManager& cm,
                         float screenWidth,
                         float screenHeight,
                         EntityId* nextId,
                         float speed = 300.0f,
                         float bulletSpeed = 500.0f,
                         float cooldown = 0.25f,
                         float bulletSize = 8.0f,
                         sf::Sound* laserSound = nullptr);

    /// @brief Processes input and updates player position / spawns bullets.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;
    float screen_width_;
    float screen_height_;
    float speed_;
    EntityId* next_entity_id_;
    float bullet_speed_;
    float fire_cooldown_;
    float bullet_size_;
    float time_since_last_shot_ = 0.0f;
    sf::Sound* laser_sound_;

    /// @brief Returns true if a key is currently pressed.
    static bool is_key_pressed(sf::Keyboard::Key key);
};

#endif // PLAYER_MOVEMENT_SYSTEM_HPP