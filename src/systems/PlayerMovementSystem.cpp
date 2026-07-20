#include <cmath>
#include "PlayerMovementSystem.hpp"
#include "ecs/Components.hpp"

PlayerMovementSystem::PlayerMovementSystem(ComponentManager& cm,
                                           float screenWidth,
                                           float screenHeight,
                                           EntityId* nextId,
                                           float speed,
                                           float bulletSpeed,
                                           float cooldown,
                                           float bulletSize)
    : cm_(cm)
    , screen_width_(screenWidth)
    , screen_height_(screenHeight)
    , speed_(speed)
    , next_entity_id_(nextId)
    , bullet_speed_(bulletSpeed)
    , fire_cooldown_(cooldown)
    , bullet_size_(bulletSize)
{
}

void PlayerMovementSystem::update(float dt)
{
    // Find the player entity (the one with PlayerTag)
    auto playerIds = cm_.get_entities_with_component<PlayerTag>();
    if (playerIds.empty())
        return;

    Entity player(playerIds[0]);

    auto* transform = cm_.get_component<Transform>(player);
    auto* velocity  = cm_.get_component<Velocity>(player);
    auto* shape     = cm_.get_component<Shape>(player);

    if (!transform || !velocity || !shape)
        return;

    // --- Input handling (movement) ---
    sf::Vector2f dir{0.0f, 0.0f};

    if (is_key_pressed(sf::Keyboard::Key::Left) ||
        is_key_pressed(sf::Keyboard::Key::A))
        dir.x -= 1.0f;

    if (is_key_pressed(sf::Keyboard::Key::Right) ||
        is_key_pressed(sf::Keyboard::Key::D))
        dir.x += 1.0f;

    if (is_key_pressed(sf::Keyboard::Key::Up) ||
        is_key_pressed(sf::Keyboard::Key::W))
        dir.y -= 1.0f;

    if (is_key_pressed(sf::Keyboard::Key::Down) ||
        is_key_pressed(sf::Keyboard::Key::S))
        dir.y += 1.0f;

    // Normalize diagonal movement
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0f)
        dir /= len;

    velocity->velocity = dir * speed_;

    // --- Update position ---
    transform->position += velocity->velocity * dt;

    // --- Clamp to screen bounds ---
    const auto& rect = shape->rect.getSize();
    float halfW = rect.x / 2.0f;
    float halfH = rect.y / 2.0f;

    if (transform->position.x < halfW)
        transform->position.x = halfW;
    if (transform->position.x > screen_width_ - halfW)
        transform->position.x = screen_width_ - halfW;
    if (transform->position.y < halfH)
        transform->position.y = halfH;
    if (transform->position.y > screen_height_ - halfH)
        transform->position.y = screen_height_ - halfH;

    // --- Shooting (Space key with cooldown) ---
    time_since_last_shot_ += dt;
    if (is_key_pressed(sf::Keyboard::Key::Space) &&
        time_since_last_shot_ >= fire_cooldown_)
    {
        time_since_last_shot_ = 0.0f;

        // Spawn bullet just above the player's top edge
        float bulletX = transform->position.x;
        float bulletY = transform->position.y - halfH - bullet_size_ / 2.0f;

        Entity bullet((*next_entity_id_)++);
        cm_.add_component(bullet, Transform{sf::Vector2f{bulletX, bulletY}});
        cm_.add_component(bullet, Velocity{sf::Vector2f{0.0f, -bullet_speed_}});
        cm_.add_component(bullet, Shape{sf::Vector2f{bullet_size_, bullet_size_}, sf::Color::Yellow});
        cm_.add_component(bullet, BulletTag{});
        cm_.add_component(bullet, Lifetime{2.0f});
    }
}

bool PlayerMovementSystem::is_key_pressed(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}