#include "Game.hpp"

Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode({width, height}), title)
{
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
    // Game logic will be added later
}

void Game::render()
{
    window_.clear(sf::Color::Black);
    window_.display();
}