# Active Context

## Current Focus
ECS-lite architecture planned and approved — directories created, next: implement features one by one in separate branches.

## What's Working
- CMake project `space-shooter` with SFML 3.0 (Graphics, Window, System, Audio, Network)
- `src/` directory structure with `main.cpp`, `Game.hpp`, `Game.cpp`
- Game class with main loop: black background window (800×600), close-event handling
- FPS counter in top-left corner (green text, Roboto font, updated every second)
- **FPS counter** in top-left corner (green text, Roboto font, updated every second)
- Successful build and link

## Active Decisions
- Project structure layout follows `.clinerules`: src/, assets/, memory-bank/, mcp-servers/, .github/workflows/
- Game logic will be separated from rendering using an ECS-like design
- All text in project must be in English
- Player sprite will be generated via image-gen MCP server (no placeholder code)
- **ECS-lite approved plan:**
  - **Components:** Position, Velocity, Sprite, Collider, Health, Lifetime, PlayerInput, EnemyTag, BulletTag
  - **Systems:** InputSystem, EnemySpawnSystem, BulletLifetimeSystem, MovementSystem, CollisionSystem, RenderSystem
  - **Entities:** Player (ID 0), Enemy (dynamic), Bullet (dynamic)
  - **Collision:** Circle-vs-circle, Bullet vs Enemy
  - **State management:** `enum class GameState { Playing, Paused, GameOver }` in Game

## Next Steps
1. ECS core: entity.hpp, component.hpp, components.hpp, system.hpp (+ skeleton .cpp files)
2. InputSystem — keyboard controls for player
3. EnemySpawnSystem — spawn enemies from top
4. BulletLifetimeSystem — remove expired bullets
5. MovementSystem — update positions
6. CollisionSystem — bullet-enemy interaction
7. RenderSystem — draw all sprites
8. PlayScene — ties all systems together, owns ECS world
9. Update Game to use PlayScene + GameState

## Current Considerations
- SFML 3.0 API differs from SFML 2.x (new event system, sf::Vector2f changes)
- Must ensure cross-platform compatibility from the start
- Each feature implemented in its own branch via git-ops MCP
- Memory Bank must never be deleted (per project rules)
- `.clinerules` updated with explicit build commands (PowerShell) and Feature Development workflow description
