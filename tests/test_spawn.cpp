#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
using Catch::Approx;
#include "ecs/Entity.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Components.hpp"
#include "systems/EnemySpawnSystem.hpp"
#include "Config.hpp"

TEST_CASE("EnemySpawnSystem spawns one enemy after interval", "[spawn]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<EnemyTag>();
    cm.register_component<Health>();
    cm.register_component<Shape>();
    cm.register_component<Zigzag>();

    EntityId nextId = 1;

    // Spawn interval = 1.0s, speed = 150.0, screen 800x600, spawnChance=0 → only regular enemies
    EnemySpawnSystem system(cm, 800.0f, 600.0f, &nextId, 1.0f, 150.0f, nullptr, nullptr, 0.0f);

    // Update with less than interval — no spawn
    system.update(0.5f);
    auto enemies = cm.get_entities_with_component<EnemyTag>();
    REQUIRE(enemies.empty());

    // Update passes the 1s threshold — one spawn
    system.update(0.6f);
    enemies = cm.get_entities_with_component<EnemyTag>();
    REQUIRE(enemies.size() == 1);
}

TEST_CASE("Spawned enemy has correct components and starting position", "[spawn]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<EnemyTag>();
    cm.register_component<Health>();
    cm.register_component<Shape>();
    cm.register_component<Zigzag>();

    EntityId nextId = 1;

    float screenW = 800.0f;
    float screenH = 600.0f;

    EnemySpawnSystem system(cm, screenW, screenH, &nextId, 0.5f, 150.0f, nullptr, nullptr, 0.0f);

    // Force a spawn
    system.update(1.0f);
    auto enemies = cm.get_entities_with_component<EnemyTag>();
    REQUIRE(enemies.size() == 1);

    Entity enemy(enemies[0]);

    // Has Transform, Velocity, Health, Shape
    auto* transform = cm.get_component<Transform>(enemy);
    auto* velocity  = cm.get_component<Velocity>(enemy);
    auto* health    = cm.get_component<Health>(enemy);
    auto* shape     = cm.get_component<Shape>(enemy);

    REQUIRE(transform != nullptr);
    REQUIRE(velocity != nullptr);
    REQUIRE(health != nullptr);
    REQUIRE(shape != nullptr);

    // Position: y = -enemyHeight (above screen)
    REQUIRE(transform->position.y == -config::enemy::size_y);
    // x should be within screen bounds (clamped)
    REQUIRE(transform->position.x >= config::enemy::size_x / 2.0f);
    REQUIRE(transform->position.x <= screenW - config::enemy::size_x / 2.0f);

    // Velocity: downward at configured speed
    REQUIRE(velocity->velocity.x == Approx(0.0f));
    REQUIRE(velocity->velocity.y == Approx(config::enemy::speed));

    // Health at configured value
    REQUIRE(health->hp == config::enemy::hp);

    // Shape size matches config
    REQUIRE(shape->rect.getSize().x == Approx(config::enemy::size_x));
    REQUIRE(shape->rect.getSize().y == Approx(config::enemy::size_y));
}

TEST_CASE("EnemySpawnSystem spawns multiple enemies over time", "[spawn]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<EnemyTag>();
    cm.register_component<Health>();
    cm.register_component<Shape>();
    cm.register_component<Zigzag>();

    EntityId nextId = 1;

    EnemySpawnSystem system(cm, 800.0f, 600.0f, &nextId, 1.0f, 150.0f, nullptr, nullptr, 0.0f);

    // 3 updates of 1s each with 1s interval → 3 spawns
    system.update(1.0f);
    system.update(1.0f);
    system.update(1.0f);
    auto enemies = cm.get_entities_with_component<EnemyTag>();
    REQUIRE(enemies.size() == 3);
}
