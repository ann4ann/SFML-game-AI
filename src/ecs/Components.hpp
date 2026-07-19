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

#endif // COMPONENTS_HPP