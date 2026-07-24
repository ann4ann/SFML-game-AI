#ifndef GAME_OVER_SYSTEM_HPP
#define GAME_OVER_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include "ecs/System.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Entity.hpp"

/// @brief Detects game-over conditions: enemies reaching the bottom of the screen
/// or colliding with the player. When triggered, sets an external game-over flag
/// so the Game class can stop the game loop and display the game-over screen.
class GameOverSystem : public System {
public:
    /// @param cm            Reference to the component manager.
    /// @param screenHeight  Screen height in pixels (bottom boundary).
    /// @param gameOver      Pointer to the external game-over flag (set to true when triggered).
    GameOverSystem(ComponentManager& cm,
                   float screenHeight,
                   bool* gameOver);

    /// @brief Checks all enemies for bottom-boundary breach and player-enemy collision.
    /// @param dt Delta time in seconds (unused — checks are frame-independent).
    void update(float dt) override;

private:
    ComponentManager& cm_;
    float screen_height_;
    bool* game_over_;

    /// @brief Checks if any enemy has reached or passed the bottom of the screen.
    void check_enemy_bottom();

    /// @brief Checks if any enemy's bounding box intersects the player's bounding box.
    void check_player_enemy_collision();
};

#endif // GAME_OVER_SYSTEM_HPP
