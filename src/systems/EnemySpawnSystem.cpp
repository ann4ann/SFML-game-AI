#include "EnemySpawnSystem.hpp"
#include "Config.hpp"
#include "ecs/Components.hpp"

EnemySpawnSystem::EnemySpawnSystem(ComponentManager& cm,
                                   float screenWidth,
                                   float screenHeight,
                                   EntityId* nextId,
                                   float interval,
                                   float speed,
                                   std::shared_ptr<sf::Texture> texture,
                                   std::shared_ptr<sf::Texture> zigzagTexture,
                                   float spawnChance)
    : cm_(cm)
    , screen_width_(screenWidth)
    , screen_height_(screenHeight)
    , spawn_interval_(interval)
    , enemy_speed_(speed)
    , spawn_timer_(0.0f)
    , zigzag_spawn_chance_(spawnChance)
    , next_entity_id_(nextId)
    , enemy_texture_(std::move(texture))
    , zigzag_texture_(std::move(zigzagTexture))
    , rng_(std::random_device{}())
    , x_dist_(0.0f, screenWidth)
    , type_dist_(0.0f, 1.0f)
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

    // Roll for zigzag vs regular enemy
    if (type_dist_(rng_) < zigzag_spawn_chance_)
    {
        spawn_zigzag_enemy(x);
        return;
    }

    // Start just above the screen
    float y = -enemyHeight;

    // Create entity using the shared ID counter from Game
    Entity enemy((*next_entity_id_)++);

    cm_.add_component(enemy, Transform{sf::Vector2f{x, y}});
    cm_.add_component(enemy, Velocity{sf::Vector2f{0.0f, enemy_speed_}});
    cm_.add_component(enemy, EnemyTag{});
    cm_.add_component(enemy, Health{config::enemy::hp});

    // Shape is always added for collision hitbox (CollisionSystem uses Shape for AABB).
    // When texture is loaded, Sprite is drawn in render() instead.
    cm_.add_component(enemy, Shape{sf::Vector2f{enemyWidth, enemyHeight}, sf::Color::Transparent});

    if (enemy_texture_)
    {
        // Use sprite rendering with the shared texture.
        // Generated 2026-07-23 with prompt: "A hostile, mouse-themed alien drone, pixel art,
        // top-down view, red and dark gray colors, menacing, small size, transparent background preferred"
        cm_.add_component(enemy, Sprite{enemy_texture_, sf::Vector2f{enemyWidth, enemyHeight}});
    }
}

void EnemySpawnSystem::spawn_zigzag_enemy(float x)
{
    const float enemyWidth  = config::zigzag_enemy::size_x;
    const float enemyHeight = config::zigzag_enemy::size_y;
    const float y = -enemyHeight;

    Entity enemy((*next_entity_id_)++);

    cm_.add_component(enemy, Transform{sf::Vector2f{x, y}});
    // Initial velocity: downward only; ZigzagSystem will set horizontal component each frame
    cm_.add_component(enemy, Velocity{sf::Vector2f{0.0f, config::zigzag_enemy::speed}});
    cm_.add_component(enemy, EnemyTag{});
    cm_.add_component(enemy, Health{config::zigzag_enemy::hp});
    cm_.add_component(enemy, Zigzag{
        config::zigzag_enemy::amplitude,
        config::zigzag_enemy::frequency,
        x
    });

    // Shape for collision hitbox
    cm_.add_component(enemy, Shape{sf::Vector2f{enemyWidth, enemyHeight}, sf::Color::Transparent});

    if (zigzag_texture_)
    {
        // Use sprite rendering with the shared zigzag texture.
        // Generated 2026-07-28 with prompt: "A hostile pixel art alien with a serpentine or snake-like
        // design, top-down view, electric green and dark purple colors, zigzag pattern on body,
        // transparent background"
        cm_.add_component(enemy, Sprite{zigzag_texture_, sf::Vector2f{enemyWidth, enemyHeight}});
    }
}
