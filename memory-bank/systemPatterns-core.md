# System Patterns — Core (Invariants)

> **Purpose**: Architecture invariants needed for almost any feature. Short and stable. Current component/system inventory moved to `systemPatterns-currentSet.md`.

## Architecture Overview
ECS-like (Entity-Component-System) design separating game logic from rendering.

## ECS-lite Invariants

- **Entities** — lightweight `EntityId` (uint32_t) wrappers; no data stored in entity objects. `INVALID_ENTITY = 0`.
- **Components** — plain data structs inheriting `Component`. Stored externally in `ComponentManager`.
- **ComponentManager** — type-erased `unordered_map<EntityId, T>` per component type. Methods: `register_component<T>()`, `add_component<T>()`, `get_component<T>()`, `has_component<T>()`, `remove_component<T>()`, `get_entities_with_component<T>()`.
- **Systems** — classes inheriting `System` with `virtual void update(float dt) = 0`. Operate on entity snapshots from pools.
- **Two-phase removal** — collect pending removals first, apply after iteration (avoids iterator invalidation).
- **Entity ID counter** — `Game::next_entity_id_` is the single shared counter. Systems receive `EntityId*` pointer.

## Game Loop
```
process_events() → update(dt) → render()
```
- `dt` = delta time in seconds (from `sf::Clock`)
- Systems updated in registration order (defined in `Game` constructor)

## ECS Layer Contract (where things live)

| Layer | Location | Interaction |
|-------|----------|-------------|
| Entity IDs | `src/ecs/Entity.hpp` | `EntityId` = uint32_t, shared counter in `Game` |
| Components | `src/ecs/Components.hpp` | Data structs, added/removed via `ComponentManager` |
| Component pools | `src/ecs/ComponentManager.hpp` | Type-erased maps, one per component type |
| Systems | `src/systems/` | Inherit `System`, `update(float dt)`, operate on `ComponentManager` |
| Game orchestration | `src/Game.{hpp,cpp}` | Owns `ComponentManager`, systems vector, entity counter, assets, render loop |

## Naming Conventions
- **Classes**: `CamelCase` (e.g., `CollisionSystem`, `ComponentManager`)
- **Functions/Variables**: `snake_case` (e.g., `remove_bullet_components()`, `spawn_timer_`)
- **Files**: `snake_case` (e.g., `collision_system.cpp`)
- **Documentation**: `///` Doxygen-style comments on all public headers
