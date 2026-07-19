#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "ecs/ComponentManager.hpp"
#include "ecs/System.hpp"

/// @brief Main game class that manages the game loop and state transitions.
class Game {
public:
    /// @brief Constructs the game window.
    /// @param width  Window width in pixels.
    /// @param height Window height in pixels.
    /// @param title  Window title.
    Game(unsigned int width, unsigned int height, const std::string& title);

    /// @brief Runs the main game loop.
    void run();

private:
    sf::RenderWindow window_;
    bool running_ = false;
    unsigned int screen_width_;
    unsigned int screen_height_;

    // --- ECS ---
    ComponentManager cm_;
    std::vector<std::unique_ptr<System>> systems_;

    /// @brief Creates the player entity with required components.
    void create_player();

    /// @brief Processes window events (e.g., close).
    void process_events();

    /// @brief Updates game logic.
    void update(float dt);

    /// @brief Renders the current frame.
    void render();

    // --- FPS counter ---
    sf::Font font_;
    sf::Text fps_text_{font_};
    sf::Clock fps_clock_;
    unsigned int frame_count_ = 0;
    float fps_ = 0.0f;
};

#endif // GAME_HPP