#include "ZigzagSystem.hpp"
#include "ecs/Components.hpp"
#include <cmath>

ZigzagSystem::ZigzagSystem(ComponentManager& cm)
    : cm_(cm)
{
}

void ZigzagSystem::update(float dt)
{
    auto entities = cm_.get_entities_with_component<Zigzag>();

    for (auto eid : entities)
    {
        const Entity entity(eid);

        auto* transform = cm_.get_component<Transform>(entity);
        auto* velocity  = cm_.get_component<Velocity>(entity);
        auto* zigzag    = cm_.get_component<Zigzag>(entity);

        if (!transform || !velocity || !zigzag)
            continue;

        // Update elapsed time
        zigzag->elapsed += dt;

        // Calculate horizontal velocity: amplitude * cos(freq * elapsed * 2π)
        const float omega = zigzag->frequency * 2.0f * 3.14159265f;
        velocity->velocity.x = zigzag->amplitude * std::cos(omega * zigzag->elapsed);
    }
}