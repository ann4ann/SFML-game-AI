#include "EnemySpawnSystem.hpp"
#include "Config.hpp"
#include "ecs/Components.hpp"

EnemySpawnSystem::EnemySpawnSystem(ComponentManager& cm,
                                   float screenWidth,
                                   float screenHeight,
                                   EntityId* nextId,
                                   float interval,
                                   float speed,
                                   std::shared_ptr<sf::Texture> texture)
    : cm_(cm)
    , screen_width_(screenWidth)
    , screen_height_(screenHeight)
    , spawn_interval_(interval)
    , enemy_speed_(speed)
    , next_entity_id_(nextId)
    , enemy_texture_(std::move(texture))
    , rng_(std::random_device{}())
    , x_dist_(0.0f, screenWidth)
{
}

void EnemySpawnSystem::update(float dt)
{
    spawn_timer_ += dt;
    if (spawn_timer_ >= spawn_interval_)
    {
        spawn_timer_ -= spawn_interval_;
        spawn_enemy();
    }
}

void EnemySpawnSystem::spawn_enemy()
{
    // Enemy dimensions
    const float enemyWidth  = config::enemy::size_x;
    const float enemyHeight = config::enemy::size_y;

    // Random X position, clamped so the enemy is fully within horizontal bounds
    float x = x_dist_(rng_);
    x = std::max(enemyWidth / 2.0f, std::min(x, screen_width_ - enemyWidth / 2.0f));

    // Start just above the screen
    float y = -enemyHeight;

    // Create entity using the shared ID counter from Game
    Entity enemy((*next_entity_id_)++);

    cm_.add_component(enemy, Transform{sf::Vector2f{x, y}});
    cm_.add_component(enemy, Velocity{sf::Vector2f{0.0f, enemy_speed_}});
    cm_.add_component(enemy, EnemyTag{});
    cm_.add_component(enemy, Health{config::enemy::hp});

    if (enemy_texture_)
    {
        // Use sprite rendering with the shared texture.
        // Generated 2026-07-23 with prompt: "A hostile, mouse-themed alien drone, pixel art,
        // top-down view, red and dark gray colors, menacing, small size, transparent background preferred"
        cm_.add_component(enemy, Sprite{enemy_texture_, sf::Vector2f{enemyWidth, enemyHeight}});
    }
    else
    {
        // Fallback to rectangle if no texture was loaded.
        cm_.add_component(enemy, Shape{sf::Vector2f{enemyWidth, enemyHeight}, sf::Color::Red});
    }
}