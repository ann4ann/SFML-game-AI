#include "GameOverSystem.hpp"
#include "ecs/Components.hpp"

GameOverSystem::GameOverSystem(ComponentManager& cm,
                               float screenHeight,
                               bool* gameOver,
                               int* lives)
    : cm_(cm)
    , screen_height_(screenHeight)
    , game_over_(gameOver)
    , lives_(lives)
{
}

void GameOverSystem::update(float dt)
{
    (void)dt;  // Unused — checks are frame-independent

    if (!game_over_ || *game_over_)
        return;  // No flag pointer or already game over

    check_enemy_bottom();
    if (*game_over_)
        return;

    check_player_enemy_collision();
}

void GameOverSystem::check_enemy_bottom()
{
    auto enemyIds = cm_.get_entities_with_component<EnemyTag>();
    for (auto eid : enemyIds)
    {
        Entity enemy(eid);
        auto* transform = cm_.get_component<Transform>(enemy);
        auto* shape = cm_.get_component<Shape>(enemy);

        if (!transform || !shape)
            continue;

        // Shape has centered origin, so bottom edge = position.y + size.y / 2
        float enemy_bottom = transform->position.y + shape->rect.getSize().y * 0.5f;
        if (enemy_bottom >= screen_height_)
        {
            // Lose a life when an enemy reaches the bottom
            if (lives_ && *lives_ > 0)
            {
                *lives_ -= 1;
            }

            // Remove the enemy
            remove_enemy_components(enemy);

            // Game over if no lives remain
            if (lives_ && *lives_ <= 0)
            {
                *game_over_ = true;
            }
            return;
        }
    }
}

void GameOverSystem::check_player_enemy_collision()
{
    auto playerIds = cm_.get_entities_with_component<PlayerTag>();
    if (playerIds.empty())
        return;

    Entity player(playerIds[0]);
    auto* player_transform = cm_.get_component<Transform>(player);
    auto* player_shape = cm_.get_component<Shape>(player);

    if (!player_transform || !player_shape)
        return;

    sf::Vector2f player_size = player_shape->rect.getSize();
    // Shape has centered origin — compute AABB accordingly
    float player_left   = player_transform->position.x - player_size.x * 0.5f;
    float player_top    = player_transform->position.y - player_size.y * 0.5f;
    float player_right  = player_transform->position.x + player_size.x * 0.5f;
    float player_bottom = player_transform->position.y + player_size.y * 0.5f;

    auto enemyIds = cm_.get_entities_with_component<EnemyTag>();
    for (auto eid : enemyIds)
    {
        Entity enemy(eid);
        auto* enemy_transform = cm_.get_component<Transform>(enemy);
        auto* enemy_shape = cm_.get_component<Shape>(enemy);

        if (!enemy_transform || !enemy_shape)
            continue;

        sf::Vector2f enemy_size = enemy_shape->rect.getSize();
        float enemy_left   = enemy_transform->position.x - enemy_size.x * 0.5f;
        float enemy_top    = enemy_transform->position.y - enemy_size.y * 0.5f;
        float enemy_right  = enemy_transform->position.x + enemy_size.x * 0.5f;
        float enemy_bottom = enemy_transform->position.y + enemy_size.y * 0.5f;

        // AABB intersection test
        if (player_left < enemy_right && player_right > enemy_left &&
            player_top < enemy_bottom && player_bottom > enemy_top)
        {
            // Lose a life on enemy collision
            if (lives_ && *lives_ > 0)
            {
                *lives_ -= 1;
            }

            // Remove the enemy so it doesn't keep dealing damage
            remove_enemy_components(enemy);

            // Game over if no lives remain
            if (lives_ && *lives_ <= 0)
            {
                *game_over_ = true;
            }
            return;
        }
    }
}

void GameOverSystem::remove_enemy_components(Entity entity)
{
    cm_.remove_component<Transform>(entity);
    cm_.remove_component<Velocity>(entity);
    cm_.remove_component<Shape>(entity);
    cm_.remove_component<Sprite>(entity);
    cm_.remove_component<EnemyTag>(entity);
    cm_.remove_component<Health>(entity);
}
