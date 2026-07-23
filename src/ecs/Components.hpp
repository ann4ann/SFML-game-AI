#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <memory>
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

/// @brief Sprite component for textured rendering.
/// Stores a shared texture reference and a drawable sprite.
/// Multiple entities can share the same texture via shared_ptr.
struct Sprite : public Component {
    std::shared_ptr<sf::Texture> texture;       ///< Shared texture (may be null for fallback).
    std::unique_ptr<sf::Sprite> sprite;         ///< Drawable sprite (nullptr if fallback).

    Sprite() = default;

    /// @brief Construct a sprite from a texture and scale it to the target size.
    /// @param tex        Shared texture to use.
    /// @param targetSize Desired size in pixels (sprite will be scaled to fit).
    Sprite(std::shared_ptr<sf::Texture> tex, const sf::Vector2f& targetSize)
        : texture(std::move(tex))
        , sprite(std::make_unique<sf::Sprite>(*texture))
    {
        const sf::Vector2f texSize(static_cast<float>(texture->getSize().x),
                                   static_cast<float>(texture->getSize().y));
        const float scaleX = targetSize.x / texSize.x;
        const float scaleY = targetSize.y / texSize.y;
        sprite->setScale({scaleX, scaleY});
        sprite->setOrigin({texSize.x * 0.5f, texSize.y * 0.5f});
    }

    // Move-only: unique_ptr disables copying.
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;
    Sprite(Sprite&&) = default;
    Sprite& operator=(Sprite&&) = default;
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
        rect.setOrigin(size / 2.0f);
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