# Progress

## In Progress
- [ ] Sounds, effects and final visual polish

## Completed
- [x] Project scaffolding (CMake + SFML 3.0)
- [x] Game class with main loop + FPS counter
- [x] ECS core (ComponentManager, Entity, System)
- [x] PlayerMovementSystem (movement + Space to fire)
- [x] EnemySpawnSystem (timer-based, random X)
- [x] MovementSystem (velocity * dt for non-player)
- [x] CollisionSystem (bullet vs enemy AABB, deferred removal)
- [x] BulletCleanupSystem (off-screen + lifetime)
- [x] Scoring system — +10 points per enemy, displayed in top-left corner
- [x] Game balance config — all magic numbers extracted into `Config.hpp`
- [x] Image-gen MCP server — Gen-API Flux-2, README
- [x] Player ship sprite — textured sprite (AI-generated player_ship.png)
- [x] Enemy drone sprite — Sprite ECS component, textured enemy (AI-generated enemy_drone.png)
- [x] Scrolling starfield background with infinite vertical scroll

## Backlog
1. Main menu / Game Over scenes
2. Player lives / game over condition
3. Sound effects (procedural or API)

## Known Issues
- Entity without all components silently skipped in render — no error feedback
- CollisionSystem O(n*m) per frame — acceptable for current scale
- Enemy HP=1 means single-bullet kill; journal initially mis-reported as 2