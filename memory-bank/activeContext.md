# Active Context

## Current Focus
Bullet shooting system implemented. Player fires yellow square bullets with Space (250ms cooldown). Bullets are auto-removed when off-screen or after 2s lifetime. Next: collision detection.

## What's Working
- CMake project `space-shooter` with SFML 3.0 (Graphics, Window, System, Audio, Network)
- `src/` directory structure with `main.cpp`, `Game.hpp`, `Game.cpp`
- Game class with main loop: black background window (800×600), close-event handling
- FPS counter in top-left corner (green text, Roboto font, updated every second)
- Successful build and link
- **ECS core implemented:**
  - `Component.hpp` — base struct + type ID generation
  - `Entity.hpp` — lightweight entity ID wrapper
  - `System.hpp` — abstract base class with `update(float dt)`
  - `ComponentManager.hpp` — type-erased component pools
- **Components:** Transform, Velocity, Shape, PlayerTag, EnemyTag, Health, BulletTag, Lifetime
- **PlayerMovementSystem** — arrow keys / WASD, diagonal normalization, screen clamping, **Space to fire bullets (250ms cooldown)**
- **Player entity** — cyan rectangle at bottom center of screen
- **EnemySpawnSystem** — timer-based spawning of red rectangles above screen, moving downward at 150 px/s
- **MovementSystem** — updates position for all non-player entities with Transform + Velocity
- **BulletCleanupSystem** — removes bullets that exit the top of the screen or exceed 2s lifetime
- **Bullet rendering** — yellow 8×8 squares drawn via BulletTag
- **Shared entity ID counter** in Game (`next_entity_id_`) prevents ID collisions between player, enemies, and bullets

## Active Decisions
- Project structure layout follows `.clinerules`: src/, assets/, memory-bank/, mcp-servers/, .github/workflows/
- Game logic separated from rendering using ECS-like design
- All text in project must be in English
- Player sprite will be generated via image-gen MCP server (no placeholder code)
- **ECS-lite plan (updated):**
  - **Components:** Transform, Velocity, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime
  - **Systems:** PlayerMovementSystem, EnemySpawnSystem, BulletCleanupSystem, MovementSystem, CollisionSystem, RenderSystem
  - **Entities:** Player (ID 1), Enemy (dynamic), Bullet (dynamic)
  - **Collision:** Bullet vs Enemy (next feature)
  - **State management:** `enum class GameState { Playing, Paused, GameOver }` in Game

## Next Steps
1. CollisionSystem — bullet-enemy interaction
2. PlayScene — ties all systems together, owns ECS world

## Current Considerations
- SFML 3.0 API differs from SFML 2.x (new event system, sf::Vector2f changes)
- Must ensure cross-platform compatibility from the start
- Each feature implemented in its own branch via git-ops MCP
- Memory Bank must never be deleted (per project rules)
- `.clinerules` updated with explicit build commands (PowerShell) and Feature Development workflow description