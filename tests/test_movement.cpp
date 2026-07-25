#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
using Catch::Approx;
#include "ecs/Entity.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/Components.hpp"
#include "systems/MovementSystem.hpp"

TEST_CASE("MovementSystem updates Transform based on Velocity", "[movement]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<PlayerTag>();

    EntityId nextId = 1;

    // Entity with both Transform and Velocity
    Entity entity1(nextId++);
    cm.add_component(entity1, Transform{sf::Vector2f{0.0f, 0.0f}});
    cm.add_component(entity1, Velocity{sf::Vector2f{100.0f, 50.0f}});

    MovementSystem system(cm);
    system.update(2.0f);

    auto* t1 = cm.get_component<Transform>(entity1);
    REQUIRE(t1 != nullptr);
    REQUIRE(t1->position.x == Approx(200.0f));
    REQUIRE(t1->position.y == Approx(100.0f));
}

TEST_CASE("MovementSystem does not move PlayerTag entities", "[movement]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<PlayerTag>();

    EntityId nextId = 1;

    // Player entity — should NOT be moved by MovementSystem
    Entity player(nextId++);
    cm.add_component(player, Transform{sf::Vector2f{10.0f, 20.0f}});
    cm.add_component(player, Velocity{sf::Vector2f{100.0f, 0.0f}});
    cm.add_component(player, PlayerTag{});

    MovementSystem system(cm);
    system.update(1.0f);

    auto* t = cm.get_component<Transform>(player);
    REQUIRE(t != nullptr);
    REQUIRE(t->position.x == Approx(10.0f));  // unchanged
    REQUIRE(t->position.y == Approx(20.0f));
}

TEST_CASE("MovementSystem handles entity without Velocity gracefully", "[movement]")
{
    ComponentManager cm;
    cm.register_component<Transform>();
    cm.register_component<Velocity>();
    cm.register_component<PlayerTag>();

    EntityId nextId = 1;

    // Entity with only Transform — should not crash or change position
    Entity entity(nextId++);
    cm.add_component(entity, Transform{sf::Vector2f{42.0f, 99.0f}});

    MovementSystem system(cm);
    system.update(5.0f);

    auto* t = cm.get_component<Transform>(entity);
    REQUIRE(t != nullptr);
    REQUIRE(t->position.x == Approx(42.0f));
    REQUIRE(t->position.y == Approx(99.0f));
}