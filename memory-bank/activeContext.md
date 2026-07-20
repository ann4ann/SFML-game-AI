# Active Context

## 🔴 CRITICAL
- **Current task**: CollisionSystem (bullet vs enemy) implemented and built. Enemy HP = 2 for testing.
- **Key decision**: Deferred removal — two-phase collision collection to avoid unordered_map iterator invalidation when removing components.

## 🟡 ACTIVE DECISIONS
- **SFML 3.0 collision API**: `getGlobalBounds()` + `findIntersection()` instead of manual AABB — simpler and more accurate
- **System order**: PlayerMovement → EnemySpawn → Movement → **CollisionSystem** → BulletCleanup (collision before off-screen cleanup)
- **One bullet = one enemy**: break after first hit; a bullet cannot hit multiple enemies per frame
- **ECS approach**: components stored in `unordered_map<EntityId, T>`, systems operate on ID-vector snapshots
- **Entity ID**: shared counter `next_entity_id_` in Game (prevents ID collisions between player, enemies, bullets)

## 🟢 CONTEXT

### What's Working
- CMake project `space-shooter` with SFML 3.0 (Graphics, Window, System), MinGW build
- Game loop: process_events → update(dt) → render, black 800×600 window, FPS counter
- **ECS**: ComponentManager (type-erased pools), Entity (ID wrapper), System (virtual update)
- **8 components**: Transform, Velocity, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime
- **5 systems**: PlayerMovement (movement + Space to fire), EnemySpawn (2s timer), Movement (velocity*dt), CollisionSystem (AABB), BulletCleanup (off-screen + lifetime)
- **Rendering**: PlayerTag (cyan), EnemyTag (red), BulletTag (yellow) via Shape::rect

### Next Steps
1. PlayScene — extract ECS from Game into a dedicated scene, prepare State Machine
2. Scoring system — points for destroying enemies
3. Asset pipeline — textures (via image-gen MCP), sounds