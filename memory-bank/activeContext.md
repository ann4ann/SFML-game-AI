# Active Context

## 🔴 CRITICAL
- **Current task**: Scoring system implemented. CollisionSystem now accepts an `int* score_` pointer and awards +10 points per destroyed enemy. Score displayed via `sf::Text` in top-left corner.
- **Key decision**: Score stored as `int score_` in Game, passed to CollisionSystem as a non-owning pointer. No separate UI entity yet — only 2 text elements (FPS + Score), not worth the overhead.

## 🟡 ACTIVE DECISIONS
- **Score via pointer**: CollisionSystem takes `int* score` (default nullptr for safety). Only modifies through `if (score_) *score_ += 10;` — minimal coupling, keeps ECS clean.
- **Score text update**: String is rebuilt every frame in `render()` from `score_` value — simple, no performance concern at this scale.
- **Declaration order**: `font_` moved before any `sf::Text` members to guarantee construction order.

## 🟢 CONTEXT

### What's Working
- CMake project `space-shooter` with SFML 3.0, MinGW build
- Game loop: process_events → update(dt) → render, black 800×600 window
- ECS: ComponentManager, Entity, System
- **8 components**: Transform, Velocity, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime
- **6 systems**: PlayerMovement, EnemySpawn, Movement, CollisionSystem (+scoring), BulletCleanup
- **Rendering**: PlayerTag (cyan), EnemyTag (red), BulletTag (yellow) via Shape::rect
- **HUD**: FPS (green, top-left), Score (white, below FPS)

### Next Steps
1. PlayScene — extract ECS from Game into a dedicated scene, prepare State Machine
2. Asset pipeline — textures (via image-gen MCP), sounds