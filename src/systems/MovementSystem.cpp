#include "MovementSystem.hpp"
#include "ecs/Components.hpp"

MovementSystem::MovementSystem(ComponentManager& cm)
    : cm_(cm)
{
}

void MovementSystem::update(float dt)
{
    // Get all entities with Transform + Velocity
    auto transformIds = cm_.get_entities_with_component<Transform>();
    auto velocityIds  = cm_.get_entities_with_component<Velocity>();

    // Build a set of entities that have both Transform and Velocity
    // (simple O(n*m) is fine for our scale)
    for (auto eid : transformIds)
    {
        Entity entity(eid);

        // Skip player — PlayerMovementSystem handles it
        if (cm_.has_component<PlayerTag>(entity))
            continue;

        auto* transform = cm_.get_component<Transform>(entity);
        auto* velocity  = cm_.get_component<Velocity>(entity);

        if (transform && velocity)
        {
            transform->position += velocity->velocity * dt;
        }
    }
}