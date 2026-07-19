#include "Game.hpp"

Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode({width, height}), title)
{
    // Load font
    if (!font_.openFromFile("assets/fonts/Roboto.ttf"))
    {
        // If font fails to load, we'll still run — FPS text will just be invisible
        // (no crash)
    }

    // Configure FPS text
    fps_text_.setFont(font_);
    fps_text_.setCharacterSize(18);
    fps_text_.setFillColor(sf::Color::Green);
    fps_text_.setPosition({10.0f, 10.0f});
    fps_text_.setString("FPS: --");

    // Start the FPS clock
    fps_clock_.restart();
}

void Game::run()
{
    running_ = true;

    while (running_ && window_.isOpen())
    {
        process_events();
        update();
        render();
    }
}

void Game::process_events()
{
    while (const auto event = window_.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            running_ = false;
            window_.close();
        }
    }
}

void Game::update()
{
    // --- FPS counter ---
    ++frame_count_;

    // Update FPS text every second
    if (fps_clock_.getElapsedTime().asSeconds() >= 1.0f)
    {
        fps_ = static_cast<float>(frame_count_);
        fps_text_.setString("FPS: " + std::to_string(static_cast<int>(fps_)));

        frame_count_ = 0;
        fps_clock_.restart();
    }
}

void Game::render()
{
    window_.clear(sf::Color::Black);

    // Draw FPS counter
    window_.draw(fps_text_);

    window_.display();
}
