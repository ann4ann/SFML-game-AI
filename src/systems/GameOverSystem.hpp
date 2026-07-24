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
    /// @param lives         Pointer to the external lives counter (decremented on player-enemy collision).
    GameOverSystem(ComponentManager& cm,
                   float screenHeight,
                   bool* gameOver,
                   int* lives);

    /// @brief Checks all enemies for bottom-boundary breach and player-enemy collision.
    /// @param dt Delta time in seconds (unused — checks are frame-independent).
    void update(float dt) override;

private:
    ComponentManager& cm_;
    float screen_height_;
    bool* game_over_;
    int* lives_;

    /// @brief Checks if any enemy has reached or passed the bottom of the screen.
    void check_enemy_bottom();

    /// @brief Checks if any enemy's bounding box intersects the player's bounding box.
    /// On collision: decrements lives, removes the enemy, and sets game_over if lives reach 0.
    void check_player_enemy_collision();

    /// @brief Removes all components associated with an enemy entity.
    /// @param entity The enemy entity to clean up.
    void remove_enemy_components(Entity entity);
};

#endif // GAME_OVER_SYSTEM_HPP
