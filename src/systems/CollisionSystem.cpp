#include "CollisionSystem.hpp"
#include "Config.hpp"
#include "ecs/Components.hpp"

CollisionSystem::CollisionSystem(ComponentManager& cm,
                                 int* score,
                                 sf::Sound* hitSound,
                                 sf::Sound* explosionSound,
                                 EntityId* nextId,
                                 std::shared_ptr<sf::Texture> explosionTexture)
    : cm_(cm)
    , score_(score)
    , hit_sound_(hitSound)
    , explosion_sound_(explosionSound)
    , next_entity_id_(nextId)
    , explosion_texture_(std::move(explosionTexture))
{
}

void CollisionSystem::update(float /*dt*/)
{
    // Phase 1: collect all bullet and enemy IDs into local vectors (snapshots)
    auto bulletIds = cm_.get_entities_with_component<BulletTag>();
    auto enemyIds  = cm_.get_entities_with_component<EnemyTag>();

    // Pending removals: pairs of (bullet_id, enemy_id) that need to be destroyed.
    // Using a vector to avoid iterator invalidation — we only modify components
    // after the detection loop finishes.
    std::vector<std::pair<EntityId, EntityId>> pendingRemovals;

    for (auto bid : bulletIds)
    {
        Entity bullet(bid);
        auto* bulletTransform = cm_.get_component<Transform>(bullet);
        auto* bulletShape     = cm_.get_component<Shape>(bullet);

        if (!bulletTransform || !bulletShape)
            continue;

        // Update the bullet's position on its shape so getGlobalBounds is correct
        bulletShape->rect.setPosition(bulletTransform->position);
        const auto bulletBounds = bulletShape->rect.getGlobalBounds();

        for (auto eid : enemyIds)
        {
            Entity enemy(eid);
            auto* enemyTransform = cm_.get_component<Transform>(enemy);
            auto* enemyShape     = cm_.get_component<Shape>(enemy);
            auto* enemyHealth    = cm_.get_component<Health>(enemy);

            if (!enemyTransform || !enemyShape || !enemyHealth)
                continue;

            // Update the enemy's position on its shape for correct bounds
            enemyShape->rect.setPosition(enemyTransform->position);
            const auto enemyBounds = enemyShape->rect.getGlobalBounds();

            // Check AABB overlap using SFML 3.0's findIntersection
            if (bulletBounds.findIntersection(enemyBounds).has_value())
            {
                pendingRemovals.emplace_back(bid, eid);
                // Break inner loop — this bullet already hit something, skip other enemies
                break;
            }
        }
    }

    // Phase 2: resolve collisions (deferred removal)
    for (const auto& [bid, eid] : pendingRemovals)
    {
        Entity bullet(bid);
        Entity enemy(eid);

        // Bullet is always destroyed on collision
        // (check if it still exists — it might have been removed via another bullet)
        if (cm_.has_component<BulletTag>(bullet))
        {
            remove_bullet_components(bullet);
        }

        // Enemy takes damage
        auto* enemyHealth = cm_.get_component<Health>(enemy);
        if (enemyHealth && cm_.has_component<EnemyTag>(enemy))
        {
            enemyHealth->hp -= 1;
            if (enemyHealth->hp <= 0)
            {
                // Play explosion sound on enemy destruction
                if (explosion_sound_) explosion_sound_->play();

                // Spawn explosion animation at the enemy's position
                auto* enemyTransform = cm_.get_component<Transform>(enemy);
                if (enemyTransform)
                {
                    spawn_explosion(enemyTransform->position);
                }

                remove_enemy_components(enemy);
                // Award points for destroying an enemy
                if (score_) *score_ += config::score::points_per_kill;
            }
            else
            {
                // Play hit sound on non-lethal damage
                if (hit_sound_) hit_sound_->play();
            }
        }
    }
}

void CollisionSystem::spawn_explosion(const sf::Vector2f& position)
{
    if (!next_entity_id_ || !explosion_texture_)
        return;

    Entity explosion((*next_entity_id_)++);

    cm_.add_component(explosion, Transform{position});

    // Create sprite manually — do NOT use the scaling constructor because it
    // would scale the entire spritesheet down to the target size, making the
    // subsequent textureRect incorrect.
    auto spriteComp = Sprite();
    spriteComp.texture = explosion_texture_;
    spriteComp.sprite = std::make_unique<sf::Sprite>(*explosion_texture_);

    // Set texture rect to the first frame (left=0)
    const int fp = config::explosion::frame_pixel;
    spriteComp.sprite->setTextureRect({
        {0, 0},
        {fp, fp}
    });

    // Scale so that frame_pixel → sprite_size
    const float scale = config::explosion::sprite_size / static_cast<float>(fp);
    spriteComp.sprite->setScale({scale, scale});

    // Set origin to the centre of the frame
    spriteComp.sprite->setOrigin({fp * 0.5f, fp * 0.5f});

    cm_.add_component(explosion, std::move(spriteComp));

    // Add animation component
    cm_.add_component(explosion, ExplosionAnim{
        config::explosion::frame_count,
        config::explosion::frame_duration
    });
}

void CollisionSystem::remove_bullet_components(Entity entity)
{
    cm_.remove_component<Transform>(entity);
    cm_.remove_component<Velocity>(entity);
    cm_.remove_component<Shape>(entity);
    cm_.remove_component<BulletTag>(entity);
    cm_.remove_component<Lifetime>(entity);
}

void CollisionSystem::remove_enemy_components(Entity entity)
{
    cm_.remove_component<Transform>(entity);
    cm_.remove_component<Velocity>(entity);
    cm_.remove_component<Shape>(entity);
    cm_.remove_component<Sprite>(entity);
    cm_.remove_component<EnemyTag>(entity);
    cm_.remove_component<Health>(entity);
}