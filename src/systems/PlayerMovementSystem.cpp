#include <cmath>
#include "PlayerMovementSystem.hpp"
#include "ecs/Components.hpp"

PlayerMovementSystem::PlayerMovementSystem(ComponentManager& cm,
                                           float screenWidth,
                                           float screenHeight,
                                           float speed)
    : cm_(cm)
    , screen_width_(screenWidth)
    , screen_height_(screenHeight)
    , speed_(speed)
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

    // --- Input handling ---
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
}

bool PlayerMovementSystem::is_key_pressed(sf::Keyboard::Key key)
{
    return sf::Keyboard::isKeyPressed(key);
}