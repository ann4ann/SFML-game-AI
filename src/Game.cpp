#include "Game.hpp"
#include "Config.hpp"
#include "ecs/Components.hpp"
#include "systems/PlayerMovementSystem.hpp"
#include "systems/EnemySpawnSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/BulletCleanupSystem.hpp"
#include "systems/CollisionSystem.hpp"

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

    // Configure Score text
    score_text_.setFont(font_);
    score_text_.setCharacterSize(18);
    score_text_.setFillColor(sf::Color::White);
    score_text_.setPosition({10.0f, 34.0f});
    score_text_.setString("Score: 0");

    // Start the FPS clock
    fps_clock_.restart();

    // --- Load sound effects ---
    // Generated 2026-07-23 via sound-gen MCP (laser type, 0.3s)
    if (laser_buffer_.loadFromFile("assets/sounds/player_laser.wav"))
        laser_sound_ = std::make_unique<sf::Sound>(laser_buffer_);
    else
        sf::err() << "Warning: Could not load 'assets/sounds/player_laser.wav'\n";

    // Generated 2026-07-23 via sound-gen MCP (hit type, 0.2s)
    if (hit_buffer_.loadFromFile("assets/sounds/enemy_hit.wav"))
        hit_sound_ = std::make_unique<sf::Sound>(hit_buffer_);
    else
        sf::err() << "Warning: Could not load 'assets/sounds/enemy_hit.wav'\n";

    // Generated 2026-07-23 via sound-gen MCP (explosion type, 0.5s)
    if (explosion_buffer_.loadFromFile("assets/sounds/enemy_explosion.wav"))
        explosion_sound_ = std::make_unique<sf::Sound>(explosion_buffer_);
    else
        sf::err() << "Warning: Could not load 'assets/sounds/enemy_explosion.wav'\n";

    // --- Register component types ---
    cm_.register_component<Transform>();
    cm_.register_component<Velocity>();
    cm_.register_component<Shape>();
    cm_.register_component<Sprite>();
    cm_.register_component<PlayerTag>();
    cm_.register_component<EnemyTag>();
    cm_.register_component<Health>();
    cm_.register_component<BulletTag>();
    cm_.register_component<Lifetime>();

    // --- Create player ---
    create_player();

    // --- Register systems ---
    // --- Load starfield background texture ---
    // Generated 2026-07-23 with prompt: "A seamless starfield texture for a space game,
    // dark blue background with scattered white stars of varying sizes, pixel art, tiling"
    if (starfield_texture_.loadFromFile("assets/imgs/starfield.png"))
    {
        starfield_texture_.setRepeated(true);

        // Create both sprites
        starfield_sprite1_ = std::make_unique<sf::Sprite>(starfield_texture_);
        starfield_sprite2_ = std::make_unique<sf::Sprite>(starfield_texture_);

        // Texture is 512×512; set texture rect to cover full screen width.
        // sprite1 at (0, 0), sprite2 at (0, -screen_height) — one above the other.
        starfield_sprite1_->setTextureRect({
            {0, 0},
            {static_cast<int>(screen_width_), static_cast<int>(screen_height_)}
        });
        starfield_sprite2_->setTextureRect({
            {0, 0},
            {static_cast<int>(screen_width_), static_cast<int>(screen_height_)}
        });
        starfield_sprite2_->setPosition({0.0f, -static_cast<float>(screen_height_)});
    }
    else
    {
        sf::err() << "Warning: Could not load starfield texture 'assets/imgs/starfield.png', "
                  << "background will be black.\n";
    }

    systems_.push_back(std::make_unique<PlayerMovementSystem>(
        cm_,
        static_cast<float>(screen_width_),
        static_cast<float>(screen_height_),
        &next_entity_id_,
        config::player::speed,
        config::bullet::speed,
        config::bullet::cooldown,
        config::bullet::size,
        laser_sound_.get()
    ));

    // Try to load enemy texture for sprite rendering
    // Generated 2026-07-23 with prompt: "A hostile, mouse-themed alien drone, pixel art,
    // top-down view, red and dark gray colors, menacing, small size, transparent background preferred"
    if (enemy_texture_.loadFromFile("assets/imgs/enemy_drone.png"))
    {
        enemy_texture_.setSmooth(true);
        enemy_tex_shared_ = std::make_shared<sf::Texture>(enemy_texture_);
        use_enemy_texture_fallback_ = false;
    }
    else
    {
        sf::err() << "Warning: Could not load enemy texture 'assets/imgs/enemy_drone.png', "
                  << "using fallback rectangle.\n";
    }

    systems_.push_back(std::make_unique<EnemySpawnSystem>(
        cm_,
        static_cast<float>(screen_width_),
        static_cast<float>(screen_height_),
        &next_entity_id_,
        config::enemy::spawn_interval,
        config::enemy::speed,
        enemy_tex_shared_
    ));

    systems_.push_back(std::make_unique<MovementSystem>(cm_));

    systems_.push_back(std::make_unique<CollisionSystem>(
        cm_,
        &score_,
        hit_sound_.get(),
        explosion_sound_.get()
    ));

    systems_.push_back(std::make_unique<BulletCleanupSystem>(
        cm_,
        static_cast<float>(screen_height_)
    ));
}

void Game::create_player()
{
    Entity player(next_entity_id());

    // Center the player at the bottom of the screen
    float playerWidth  = config::player::size_x;
    float playerHeight = config::player::size_y;
    float startX = (static_cast<float>(screen_width_)  - playerWidth)  / 2.0f;
    float startY =  static_cast<float>(screen_height_) - playerHeight - 20.0f;

    cm_.add_component(player, Transform{sf::Vector2f{startX, startY}});
    cm_.add_component(player, Velocity{sf::Vector2f{0.0f, 0.0f}});
    cm_.add_component(player, PlayerTag{});
    // Shape is always added for clamping / hitbox calculations in PlayerMovementSystem.
    // When texture is loaded, sprite is drawn in render() instead.
    cm_.add_component(player, Shape{sf::Vector2f{playerWidth, playerHeight}, sf::Color::Transparent});

    // Try to load the player ship texture
    // Generated 2026-07-23 with prompt: "A sleek, cat-themed space fighter with pixel art, pixel art,
    // top-down view, blue and silver colors, 8-bit style, dark background not needed"
    if (player_texture_.loadFromFile("assets/imgs/player_ship.png"))
    {
        player_texture_.setSmooth(true);
        player_sprite_ = std::make_unique<sf::Sprite>(player_texture_);

        // Scale sprite to match desired player size
        sf::Vector2f texSize(player_texture_.getSize().x, player_texture_.getSize().y);
        float scaleX = playerWidth  / texSize.x;
        float scaleY = playerHeight / texSize.y;
        player_sprite_->setScale({scaleX, scaleY});

        // Center the sprite origin
        player_sprite_->setOrigin({texSize.x * 0.5f, texSize.y * 0.5f});

        use_texture_fallback_ = false;   // Use sprite rendering
    }
    else
    {
        sf::err() << "Warning: Could not load player texture 'assets/imgs/player_ship.png', "
                  << "using fallback rectangle.\n";
        // Shape already added above; just change its color to visible
        auto* existing_shape = cm_.get_component<Shape>(player);
        if (existing_shape)
            existing_shape->rect.setFillColor(sf::Color::Cyan);
        use_texture_fallback_ = true;    // Fallback to rectangle
    }
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
    // --- Update starfield scroll ---
    if (starfield_texture_.getNativeHandle() != 0)
    {
        float scroll = config::background::scroll_speed * dt;
        float screenH = static_cast<float>(screen_height_);

        // Move both sprites downward
        starfield_sprite1_->move({0.0f, scroll});
        starfield_sprite2_->move({0.0f, scroll});

        // If a sprite has scrolled completely past the bottom, reposition it above the other
        if (starfield_sprite1_->getPosition().y >= screenH)
        {
            starfield_sprite1_->setPosition({0.0f, starfield_sprite2_->getPosition().y - screenH});
        }
        if (starfield_sprite2_->getPosition().y >= screenH)
        {
            starfield_sprite2_->setPosition({0.0f, starfield_sprite1_->getPosition().y - screenH});
        }
    }

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

    // --- Draw starfield background ---
    if (starfield_texture_.getNativeHandle() != 0)
    {
        window_.draw(*starfield_sprite1_);
        window_.draw(*starfield_sprite2_);
    }

    // --- Draw player ---
    {
        auto playerIds = cm_.get_entities_with_component<PlayerTag>();
        if (!playerIds.empty())
        {
            Entity player(playerIds[0]);
            auto* transform = cm_.get_component<Transform>(player);

            if (transform)
            {
                if (!use_texture_fallback_)
                {
                    // Texture sprite rendering
                    player_sprite_->setPosition(transform->position);
                    window_.draw(*player_sprite_);
                }
                else
                {
                    // Fallback rectangle rendering
                    auto* shape = cm_.get_component<Shape>(player);
                    if (shape)
                    {
                        shape->rect.setPosition(transform->position);
                        window_.draw(shape->rect);
                    }
                }
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

            if (!transform)
                continue;

            // Try sprite rendering first, fallback to shape
            auto* sprite_comp = cm_.get_component<Sprite>(enemy);
            if (sprite_comp && sprite_comp->sprite)
            {
                sprite_comp->sprite->setPosition(transform->position);
                window_.draw(*sprite_comp->sprite);
            }
            else
            {
                auto* shape = cm_.get_component<Shape>(enemy);
                if (shape)
                {
                    shape->rect.setPosition(transform->position);
                    window_.draw(shape->rect);
                }
            }
        }
    }

    // --- Draw bullets ---
    {
        auto bulletIds = cm_.get_entities_with_component<BulletTag>();
        for (auto eid : bulletIds)
        {
            Entity bullet(eid);
            auto* transform = cm_.get_component<Transform>(bullet);
            auto* shape     = cm_.get_component<Shape>(bullet);

            if (transform && shape)
            {
                shape->rect.setPosition(transform->position);
                window_.draw(shape->rect);
            }
        }
    }

    // Draw Score
    score_text_.setString("Score: " + std::to_string(score_));
    window_.draw(score_text_);

    // Draw FPS counter
    window_.draw(fps_text_);

    window_.display();
}