# System Patterns — Current Set

> **Purpose**: Current component and system inventory. Use ONLY when explicitly asked or when it cannot be done without it. For general feature solving, rely on `PROJECT_CONTEXT.md` and `systemPatterns-core.md` invariants.

## Current Component Set

| Component | Fields | Role |
|-----------|--------|------|
| `Transform` | `sf::Vector2f position` | World position |
| `Velocity` | `sf::Vector2f velocity` | Movement per second |
| `Sprite` | `shared_ptr<sf::Texture> texture`, `unique_ptr<sf::Sprite> sprite` | Textured visual (move-only) |
| `Shape` | `sf::RectangleShape rect` | Fallback visual / hitbox |
| `PlayerTag` | *(marker)* | Identifies player |
| `EnemyTag` | *(marker)* | Identifies enemies |
| `BulletTag` | *(marker)* | Identifies bullets |
| `Health` | `int hp` | Hit points |
| `Lifetime` | `float remaining` | Auto-removal timer |
| `ExplosionAnim` | `int current_frame, total_frames`, `float frame_timer, frame_duration`, `sf::Vector2i frame_size`, `bool remove_on_finish` | Frame-based animation |

## Current System Set

| # | System | Responsibility |
|---|--------|----------------|
| 1 | `PlayerMovementSystem` | Keyboard input (arrows + WASD), diagonal normalization, screen clamping, fire bullets (Space, 250ms cooldown) |
| 2 | `EnemySpawnSystem` | Timer-based spawning (2s interval), random X above screen, downward 150px/s |
| 3 | `MovementSystem` | `position += velocity * dt` for all non-player entities with Transform + Velocity |
| 4 | `CollisionSystem` | AABB via SFML 3.0 `findIntersection()`, deferred removal, enemy HP=2, scoring +10/kill, spawns explosions |
| 5 | `BulletCleanupSystem` | Removes bullets off-screen above top edge or expired via Lifetime component |
| 6 | `ExplosionAnimationSystem` | Frame-based spritesheet animation, auto-remove on finish |
