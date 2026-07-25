#include <catch2/catch_test_macros.hpp>
#include "ecs/Entity.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Components.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/CollisionUtils.hpp"
#include "Config.hpp"

/// Helper: create a bullet entity with a Shape at the given position.
static Entity create_bullet(ComponentManager& cm, EntityId& nextId, sf::Vector2f pos)
{
    Entity bullet(nextId++);
    cm.add_component(bullet, Transform{pos});
    cm.add_component(bullet, Velocity{sf::Vector2f{0.0f, 0.0f}});
    cm.add_component(bullet, BulletTag{});
    cm.add_component(bullet, Shape{sf::Vector2f{10.0f, 10.0f}, sf::Color::White});
    return bullet;
}

/// Helper: create an enemy entity with a Shape at the given position and HP.
static Entity create_enemy(ComponentManager& cm, EntityId& nextId, sf::Vector2f pos, int hp)
{
    Entity enemy(nextId++);
    cm.add_component(enemy, Transform{pos});
    cm.add_component(enemy, Velocity{sf::Vector2f{0.0f, 0.0f}});
    cm.add_component(enemy, EnemyTag{});
    cm.add_component(enemy, Health{hp});
    cm.add_component(enemy, Shape{sf::Vector2f{10.0f, 10.0f}, sf::Color::Red});
    return enemy;
}

TEST_CASE("aabb_overlap detects intersecting rectangles", "[collision][aabb]")
{
    sf::FloatRect a({0.0f, 0.0f}, {10.0f, 10.0f});
    sf::FloatRect b({5.0f, 5.0f}, {10.0f, 10.0f});
    REQUIRE(aabb_overlap(a, b) == true);
}

TEST_CASE("aabb_overlap returns false for non-overlapping rectangles", "[collision][aabb]")
{
    sf::FloatRect a({0.0f, 0.0f}, {10.0f, 10.0f});
    sf::FloatRect b({100.0f, 100.0f}, {10.0f, 10.0f});
    REQUIRE(aabb_overlap(a, b) == false);
}

TEST_CASE("CollisionSystem detects overlapping bullet and enemy", "[collision]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<BulletTag>();
    cm.register_component<EnemyTag>();
    cm.register_component<Health>();
    cm.register_component<Shape>();
    cm.register_component<Lifetime>();
    cm.register_component<Sprite>();

    EntityId nextId = 1;
    int score = 0;

    // Bullet at (0,0), Enemy at (5,5) — they overlap (both 10x10)
    create_bullet(cm, nextId, sf::Vector2f{0.0f, 0.0f});
    create_enemy(cm, nextId, sf::Vector2f{5.0f, 5.0f}, 1);

    CollisionSystem system(cm, &score, nullptr, nullptr, nullptr, nullptr);
    system.update(0.0f);

    // Bullet should be removed
    auto bulletIds = cm.get_entities_with_component<BulletTag>();
    REQUIRE(bulletIds.empty());

    // Enemy should be removed (HP=1, one hit)
    auto enemyIds = cm.get_entities_with_component<EnemyTag>();
    REQUIRE(enemyIds.empty());

    // Score should increase by 10
    REQUIRE(score == config::score::points_per_kill);
}

TEST_CASE("CollisionSystem does not detect non-overlapping entities", "[collision]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<BulletTag>();
    cm.register_component<EnemyTag>();
    cm.register_component<Health>();
    cm.register_component<Shape>();
    cm.register_component<Lifetime>();
    cm.register_component<Sprite>();

    EntityId nextId = 1;
    int score = 0;

    // Bullet at (0,0), Enemy at (100,100) — no overlap
    create_bullet(cm, nextId, sf::Vector2f{0.0f, 0.0f});
    create_enemy(cm, nextId, sf::Vector2f{100.0f, 100.0f}, 1);

    CollisionSystem system(cm, &score, nullptr, nullptr, nullptr, nullptr);
    system.update(0.0f);

    // Both should still exist
    auto bulletIds = cm.get_entities_with_component<BulletTag>();
    REQUIRE(bulletIds.size() == 1);
    auto enemyIds = cm.get_entities_with_component<EnemyTag>();
    REQUIRE(enemyIds.size() == 1);

    // Score unchanged
    REQUIRE(score == 0);
}

TEST_CASE("Enemy with HP=2 survives one hit", "[collision]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<BulletTag>();
    cm.register_component<EnemyTag>();
    cm.register_component<Health>();
    cm.register_component<Shape>();
    cm.register_component<Lifetime>();
    cm.register_component<Sprite>();

    EntityId nextId = 1;
    int score = 0;

    create_bullet(cm, nextId, sf::Vector2f{0.0f, 0.0f});
    Entity enemy = create_enemy(cm, nextId, sf::Vector2f{5.0f, 5.0f}, 2);

    CollisionSystem system(cm, &score, nullptr, nullptr, nullptr, nullptr);
    system.update(0.0f);

    // Bullet removed
    auto bulletIds = cm.get_entities_with_component<BulletTag>();
    REQUIRE(bulletIds.empty());

    // Enemy still exists
    auto* health = cm.get_component<Health>(enemy);
    REQUIRE(health != nullptr);
    REQUIRE(health->hp == 1);

    // Score unchanged (enemy not destroyed)
    REQUIRE(score == 0);
}
