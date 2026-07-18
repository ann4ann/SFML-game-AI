#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

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

    /// @brief Processes window events (e.g., close).
    void process_events();

    /// @brief Updates game logic (empty for now).
    void update();

    /// @brief Renders the current frame.
    void render();
};

#endif // GAME_HPP