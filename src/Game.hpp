#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <string>

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

    // --- FPS counter ---
    sf::Font font_;                         ///< Font used for FPS text.
    sf::Text fps_text_{font_};              ///< Text object displaying FPS (initialized with font ref).
    sf::Clock fps_clock_;                   ///< Clock to measure elapsed time.
    unsigned int frame_count_ = 0;          ///< Frame counter for FPS calculation.
    float fps_ = 0.0f;                      ///< Current frames per second value.
};

#endif // GAME_HPP
