# System Patterns — Core

## Architecture Overview
ECS-like (Entity-Component-System) design separating game logic from rendering.

### ECS-lite Design
- **Entities** — lightweight `EntityId` (uint32_t) wrappers; no data stored in entity objects
- **Components** — plain data structs inheriting `Component` (Transform, Velocity, Shape, tag components, Health, Lifetime)
- **ComponentManager** — type-erased `unordered_map<EntityId, T>` per component type; registration, add/get/remove/has
- **Systems** — classes inheriting `System` with `virtual void update(float dt) = 0`; operate on entity snapshots from pools
- **Two-phase removal** — collect pending removals first, apply after iteration (avoids iterator invalidation)

### Game Loop
```
process_events() → update(dt) → render()
```

### Naming Conventions
- **Classes**: `CamelCase` (e.g., `CollisionSystem`, `ComponentManager`)
- **Functions/Variables**: `snake_case` (e.g., `remove_bullet_components()`, `spawn_timer_`)
- **Files**: `snake_case` (e.g., `component_manager.hpp`, `collision_system.cpp`)
- **Documentation**: `///` Doxygen-style comments on all public headers

### Current Component Set
| Component | Fields | Role |
|-----------|--------|------|
| `Transform` | `sf::Vector2f position` | World position |
| `Velocity` | `sf::Vector2f velocity` | Movement per second |
| `Shape` | `sf::RectangleShape rect` | Visual representation |
| `PlayerTag` | *(marker)* | Identifies player |
| `EnemyTag` | *(marker)* | Identifies enemies |
| `BulletTag` | *(marker)* | Identifies bullets |
| `Health` | `int hp` | Hit points |
| `Lifetime` | `float remaining` | Auto-removal timer |

### Current System Set
1. **PlayerMovementSystem** — keyboard input (arrows + WASD), diagonal normalization, screen clamping, fire bullets (Space, 250ms cooldown)
2. **EnemySpawnSystem** — timer-based spawning of enemies at random X above screen, moving downward
3. **MovementSystem** — `position += velocity * dt` for all non-player entities with Transform + Velocity
4. **CollisionSystem** — AABB check via SFML 3.0 `getGlobalBounds()` + `findIntersection()` between bullets and enemies; deferred removal
5. **BulletCleanupSystem** — removes bullets off-screen above top edge or expired via Lifetime component