#include "ExplosionAnimationSystem.hpp"
#include "ecs/Components.hpp"

ExplosionAnimationSystem::ExplosionAnimationSystem(ComponentManager& cm)
    : cm_(cm)
{
}

void ExplosionAnimationSystem::update(float dt)
{
    // Collect snapshot of explosion entity IDs
    auto explosionIds = cm_.get_entities_with_component<ExplosionAnim>();

    // Pending removals: entities whose animation has finished
    std::vector<EntityId> pendingRemovals;

    for (auto eid : explosionIds)
    {
        Entity entity(eid);
        auto* anim   = cm_.get_component<ExplosionAnim>(entity);
        auto* sprite = cm_.get_component<Sprite>(entity);

        if (!anim || !sprite || !sprite->sprite)
            continue;

        // Advance frame timer
        anim->frame_timer += dt;
        if (anim->frame_timer >= anim->frame_duration)
        {
            anim->frame_timer -= anim->frame_duration;
            anim->current_frame++;

            if (anim->current_frame >= anim->total_frames)
            {
                // Animation complete — mark for removal
                pendingRemovals.push_back(eid);
            }
            else
            {
                // Update texture rect to show the current frame
                int left = anim->current_frame * anim->frame_size.x;
                sprite->sprite->setTextureRect({
                    {left, 0},
                    {anim->frame_size.x, anim->frame_size.y}
                });
            }
        }
    }

    // Phase 2: remove finished explosion entities
    for (auto eid : pendingRemovals)
    {
        Entity entity(eid);
        if (cm_.has_component<ExplosionAnim>(entity))
        {
            remove_explosion_components(entity);
        }
    }
}

void ExplosionAnimationSystem::remove_explosion_components(Entity entity)
{
    cm_.remove_component<Transform>(entity);
    cm_.remove_component<Sprite>(entity);
    cm_.remove_component<ExplosionAnim>(entity);
}