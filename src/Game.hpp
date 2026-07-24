#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "ecs/ComponentManager.hpp"
#include "ecs/System.hpp"
#include "ecs/Entity.hpp"

/// @brief Main game class that manages the game loop and state transitions.
class Game {
public:
    /// @brief Constructs the game window.
    /// @param width  Window width in pixels.
    /// @param height Window height in pixels.
    /// @param title  Window title.
    Game(unsigned int width, unsigned int height, const std::string& title);

    /// @brief Returns the next available entity ID.
    EntityId next_entity_id() { return next_entity_id_++; }

    /// @brief Returns the current score (for display).
    int score() const { return score_; }

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
    EntityId next_entity_id_ = 1;

    /// @brief Creates the player entity with required components.
    void create_player();

    /// @brief Processes window events (e.g., close).
    void process_events();

    /// @brief Updates game logic.
    void update(float dt);

    /// @brief Renders the current frame.
    void render();

    // --- Font (must be declared before any sf::Text) ---
    sf::Font font_;

    // --- Score ---
    int score_ = 0;
    sf::Text score_text_{font_};

    // --- FPS counter ---
    sf::Text fps_text_{font_};
    sf::Clock fps_clock_;
    unsigned int frame_count_ = 0;
    float fps_ = 0.0f;

    // --- Player sprite ---
    sf::Texture player_texture_;              ///< Player ship texture.
    std::unique_ptr<sf::Sprite> player_sprite_;   ///< Player ship sprite (nullptr if fallback).
    bool        use_texture_fallback_ = true;     ///< true → draw Shape rectangle (fallback).

    // --- Enemy texture ---
    sf::Texture enemy_texture_;                    ///< Enemy drone texture.
    std::shared_ptr<sf::Texture> enemy_tex_shared_; ///< Shared ptr for ECS (nullptr if fallback).
    bool        use_enemy_texture_fallback_ = true; ///< true → draw Shape rectangle (fallback).

    // --- Starfield background ---
    sf::Texture starfield_texture_;                     ///< Seamless starfield texture (512×512).
    std::unique_ptr<sf::Sprite> starfield_sprite1_;     ///< First  background sprite (nullptr if fallback).
    std::unique_ptr<sf::Sprite> starfield_sprite2_;     ///< Second background sprite (nullptr if fallback).

    // --- Sound effects ---
    sf::SoundBuffer laser_buffer_;                          ///< Laser shot sound buffer.
    std::unique_ptr<sf::Sound> laser_sound_;                ///< Laser shot sound (nullptr if load failed).
    sf::SoundBuffer hit_buffer_;                            ///< Enemy hit sound buffer.
    std::unique_ptr<sf::Sound> hit_sound_;                  ///< Enemy hit sound (nullptr if load failed).
    sf::SoundBuffer explosion_buffer_;                      ///< Enemy explosion sound buffer.
    std::unique_ptr<sf::Sound> explosion_sound_;            ///< Enemy explosion sound (nullptr if load failed).
};

#endif // GAME_HPP