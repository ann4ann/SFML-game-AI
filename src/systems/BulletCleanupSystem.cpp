#include "BulletCleanupSystem.hpp"
#include "ecs/Components.hpp"

BulletCleanupSystem::BulletCleanupSystem(ComponentManager& cm, float screenHeight)
    : cm_(cm)
    , screen_height_(screenHeight)
{
}

void BulletCleanupSystem::update(float dt)
{
    // Decrease lifetime for all bullets
    auto bulletIds = cm_.get_entities_with_component<BulletTag>();
    for (auto eid : bulletIds)
    {
        Entity bullet(eid);
        auto* transform = cm_.get_component<Transform>(bullet);
        auto* shape     = cm_.get_component<Shape>(bullet);
        auto* lifetime  = cm_.get_component<Lifetime>(bullet);

        // Remove bullet if off-screen (above top edge)
        // Use the bullet's bottom edge: position.y + half_height < 0
        if (transform && shape)
        {
            float halfH = shape->rect.getSize().y / 2.0f;
            if (transform->position.y + halfH < 0.0f)
            {
                remove_bullet_components(bullet);
                continue;
            }
        }

        // Remove bullet if lifetime expired
        if (lifetime)
        {
            lifetime->remaining -= dt;
            if (lifetime->remaining <= 0.0f)
            {
                remove_bullet_components(bullet);
            }
        }
    }
}

void BulletCleanupSystem::remove_bullet_components(Entity entity)
{
    cm_.remove_component<Transform>(entity);
    cm_.remove_component<Velocity>(entity);
    cm_.remove_component<Shape>(entity);
    cm_.remove_component<BulletTag>(entity);
    cm_.remove_component<Lifetime>(entity);
}