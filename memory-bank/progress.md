# Progress

## In Progress
- [ ] PlayScene — extract ECS from Game into dedicated scene, prepare state machine

## Completed
- [x] Project scaffolding (CMake + SFML 3.0)
- [x] Game class with main loop + FPS counter
- [x] ECS core (ComponentManager, Entity, System)
- [x] PlayerMovementSystem (movement + Space to fire)
- [x] EnemySpawnSystem (timer-based, random X)
- [x] MovementSystem (velocity * dt for non-player)
- [x] CollisionSystem (bullet vs enemy AABB, deferred removal)
- [x] BulletCleanupSystem (off-screen + lifetime)
- [x] Scoring system — +10 points per enemy destroyed, displayed in top-left corner

## Backlog
1. Asset pipeline (textures, sounds)
2. Main menu / Game Over scenes
3. Player lives / game over condition

## Known Issues
- Entity without all components (e.g., Shape missing) silently skipped in render — no error feedback
- CollisionSystem O(n*m) per frame — acceptable for current scale, may need spatial hash later