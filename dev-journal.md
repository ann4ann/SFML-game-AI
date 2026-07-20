# Dev Journal

## 2026-07-18 - Project scaffolding + MCP git-ops
### Done
- Created CMake project, .clinerules, Memory Bank (4 files)
- Game class: window, game loop (process/update/render)
- Custom MCP git-ops server (status, create_branch, commit, push)

## 2026-07-19 - FPS counter, ECS core, player movement, enemy spawning
### Done
- FPS counter (green, top-left), Roboto font
- ECS-lite: ComponentManager, Entity, System, 4 components, PlayerMovementSystem
- EnemySpawnSystem (timer 2s, random X, downward 150px/s) + MovementSystem
### Issues / Lessons
- Entity ID collision: shared `next_entity_id_` in Game required (separate static counters caused overwrite)

## 2026-07-20 - Bullet shooting, collision system, scoring system, game balance config
### Done
- Player shoots yellow bullets (Space, 250ms cooldown), BulletCleanupSystem (off-screen + lifetime)
- CollisionSystem: AABB via SFML 3.0 findIntersection, deferred two-phase removal, enemy HP=2
- Scoring: +10 points/enemy, `int* score` pointer in CollisionSystem, white text below FPS
- Created `Config.hpp` with constexpr constants for player, bullet, enemy, score, window
- Removed all hardcoded magic numbers from Game.cpp, system files, main.cpp

### Why it matters / What's next
- All balance values centralized in one header for easy tuning
- Next: PlayScene refactor, asset pipeline

### Issues / Lessons
- Enemy HP was mis-typed as 2 in journal (actual value was 1 in code) — fixed now in Config.hpp

