#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"

/// @brief 2D position component.
struct Transform : public Component {
    sf::Vector2f position{0.0f, 0.0f};

    Transform() = default;
    explicit Transform(const sf::Vector2f& pos) : position(pos) {}
};

/// @brief 2D velocity component (pixels per second).
struct Velocity : public Component {
    sf::Vector2f velocity{0.0f, 0.0f};

    Velocity() = default;
    explicit Velocity(const sf::Vector2f& vel) : velocity(vel) {}
};

/// @brief Rectangle shape for rendering (player, enemies, bullets).
struct Shape : public Component {
    sf::RectangleShape rect;

    Shape() = default;

    /// @brief Construct a shape with given size and color.
    /// @param size  Width and height of the rectangle.
    /// @param color Fill color.
    Shape(const sf::Vector2f& size, const sf::Color& color)
        : rect(size)
    {
        rect.setFillColor(color);
    }
};

/// @brief Tag component marking an entity as the player.
struct PlayerTag : public Component {};

/// @brief Tag component marking an entity as an enemy.
struct EnemyTag : public Component {};

/// @brief Tag component marking an entity as a bullet.
struct BulletTag : public Component {};

/// @brief Lifetime component for auto-removal after a duration.
struct Lifetime : public Component {
    float remaining = 0.0f;

    Lifetime() = default;
    explicit Lifetime(float seconds) : remaining(seconds) {}
};

/// @brief Health component for entities that can take damage.
struct Health : public Component {
    int hp = 1;

    Health() = default;
    explicit Health(int hp_) : hp(hp_) {}
};

#endif // COMPONENTS_HPP