#ifndef PLAYER_MOVEMENT_SYSTEM_HPP
#define PLAYER_MOVEMENT_SYSTEM_HPP

#include <SFML/Window/Keyboard.hpp>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"

/// @brief Handles player input and movement each frame.
/// Reads keyboard state, updates player Transform and Velocity accordingly,
/// and clamps the player inside the screen bounds.
class PlayerMovementSystem : public System {
public:
    /// @param cm          Reference to the component manager.
    /// @param screenWidth  Screen width in pixels (for clamping).
    /// @param screenHeight Screen height in pixels (for clamping).
    /// @param speed        Player movement speed in pixels per second.
    PlayerMovementSystem(ComponentManager& cm,
                         float screenWidth,
                         float screenHeight,
                         float speed = 300.0f);

    /// @brief Processes input and updates player position.
    /// @param dt Delta time in seconds.
    void update(float dt) override;

private:
    ComponentManager& cm_;
    float screen_width_;
    float screen_height_;
    float speed_;

    /// @brief Returns true if a key is currently pressed.
    static bool is_key_pressed(sf::Keyboard::Key key);
};

#endif // PLAYER_MOVEMENT_SYSTEM_HPP