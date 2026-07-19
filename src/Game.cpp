#include "Game.hpp"
#include "ecs/Components.hpp"
#include "systems/PlayerMovementSystem.hpp"
#include "systems/EnemySpawnSystem.hpp"
#include "systems/MovementSystem.hpp"

Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode({width, height}), title)
    , screen_width_(width)
    , screen_height_(height)
{
    // Load font
    if (!font_.openFromFile("assets/fonts/Roboto.ttf"))
    {
        // If font fails to load, we'll still run — FPS text will just be invisible
    }

    // Configure FPS text
    fps_text_.setFont(font_);
    fps_text_.setCharacterSize(18);
    fps_text_.setFillColor(sf::Color::Green);
    fps_text_.setPosition({10.0f, 10.0f});
    fps_text_.setString("FPS: --");

    // Start the FPS clock
    fps_clock_.restart();

    // --- Register component types ---
    cm_.register_component<Transform>();
    cm_.register_component<Velocity>();
    cm_.register_component<Shape>();
    cm_.register_component<PlayerTag>();
    cm_.register_component<EnemyTag>();
    cm_.register_component<Health>();

    // --- Create player ---
    create_player();

    // --- Register systems ---
    systems_.push_back(std::make_unique<PlayerMovementSystem>(
        cm_,
        static_cast<float>(screen_width_),
        static_cast<float>(screen_height_),
        300.0f));

    systems_.push_back(std::make_unique<EnemySpawnSystem>(
        cm_,
        static_cast<float>(screen_width_),
        static_cast<float>(screen_height_),
        &next_entity_id_,
        2.0f,   // spawn every 2 seconds
        150.0f  // downward speed
    ));

    systems_.push_back(std::make_unique<MovementSystem>(cm_));
}

void Game::create_player()
{
    Entity player(next_entity_id());

    // Center the player at the bottom of the screen
    float playerWidth  = 50.0f;
    float playerHeight = 30.0f;
    float startX = (static_cast<float>(screen_width_)  - playerWidth)  / 2.0f;
    float startY =  static_cast<float>(screen_height_) - playerHeight - 20.0f;

    cm_.add_component(player, Transform{sf::Vector2f{startX, startY}});
    cm_.add_component(player, Velocity{sf::Vector2f{0.0f, 0.0f}});
    cm_.add_component(player, Shape{sf::Vector2f{playerWidth, playerHeight}, sf::Color::Cyan});
    cm_.add_component(player, PlayerTag{});
}

void Game::run()
{
    running_ = true;
    sf::Clock delta_clock;

    while (running_ && window_.isOpen())
    {
        float dt = delta_clock.restart().asSeconds();
        process_events();
        update(dt);
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

void Game::update(float dt)
{
    // --- Update all systems ---
    for (auto& system : systems_)
    {
        system->update(dt);
    }

    // --- FPS counter ---
    ++frame_count_;
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

    // --- Draw player ---
    {
        auto playerIds = cm_.get_entities_with_component<PlayerTag>();
        if (!playerIds.empty())
        {
            Entity player(playerIds[0]);
            auto* transform = cm_.get_component<Transform>(player);
            auto* shape     = cm_.get_component<Shape>(player);

            if (transform && shape)
            {
                shape->rect.setPosition(transform->position);
                window_.draw(shape->rect);
            }
        }
    }

    // --- Draw enemies ---
    {
        auto enemyIds = cm_.get_entities_with_component<EnemyTag>();
        for (auto eid : enemyIds)
        {
            Entity enemy(eid);
            auto* transform = cm_.get_component<Transform>(enemy);
            auto* shape     = cm_.get_component<Shape>(enemy);

            if (transform && shape)
            {
                shape->rect.setPosition(transform->position);
                window_.draw(shape->rect);
            }
        }
    }

    // Draw FPS counter
    window_.draw(fps_text_);

    window_.display();
}