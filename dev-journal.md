# Dev Journal — 2026-07-18

## Summary
Initial project setup for the 2D space shooter (SFML 3.0, C++17).

## Done
- Created `CMakeLists.txt` and project has been configured:
- Created `.clinerules` and established development workflow (Plan Mode → feature branch → build-test → commit)
- Initialized **Memory Bank** with 4 files:
  - `productContext.md` — project overview, goals, planned features
  - `activeContext.md` — current focus, next steps
  - `systemPatterns.md` — ECS-like architecture, naming conventions, cross-platform strategy
  - `progress.md` — completed/in-progress/backlog tracking
- Created branch `feature/basic-project-setup`:
    - Added sources into `src/` directory
    - Added **Game class** (`Game.hpp` / `Game.cpp`) with:
        - Window creation (800×600, "Space Shooter") with black background, close-event handling
        - Main game loop (`run()` → `process_events()` / `update()` / `render()`)
        - Stub methods for future game logic
- Created branch `feature/mcp-git-ops`:
    - Implemented custom **MCP git-ops server** (`mcp-servers/git_ops/server.py`) with tools:
        - `status` — show working tree status
        - `create_branch` — create & switch to a new branch
        - `commit` — stage all changes and commit with a message
        - `push` — push current branch to origin
    - Updated `.clinerules` to mandate using git-ops MCP for all git operations

## 2026-07-19 — FPS Counter

### Summary
Added a simple FPS counter displayed in the top-left corner of the game window.

### Done
- Created branch `feature/fps-counter`
    - Added FPS counter members to `Game` class (`sf::Font`, `sf::Text`, `sf::Clock`, frame counter)
    - Loaded `assets/fonts/Roboto.ttf` in the constructor
    - FPS text rendered in green at position (10, 10), updated once per second
    - Build verified with MinGW (zero errors)
- Updated `.clinerules`:
    - Added a description of the workflow for the Feature development
    - Added project build rules (C++)

## 2026-07-19 — Player Entity & Movement System

### Summary
Implemented ECS-lite core and player movement feature. Player is a cyan rectangle at the bottom of the screen, controllable with arrow keys and WASD, clamped to screen bounds.

### Done
- Created branch `feature/player-movement`
- Added ECS core:
  - `Component.hpp` — base struct + type ID generation
  - `Entity.hpp` — lightweight entity ID wrapper
  - `System.hpp` — abstract base class with `update(float dt)`
  - `ComponentManager.hpp` — type-erased component pools with registration, add/get/remove/has
- Added components: `Transform`, `Velocity`, `Shape`, `PlayerTag`
- Added `PlayerMovementSystem`:
  - Reads keyboard state (arrows + WASD)
  - Normalizes diagonal movement
  - Updates position with delta time
  - Clamps player inside screen bounds using shape half-size
- Integrated into `Game` class:
  - Component registration in constructor
  - Player creation (`create_player()`) centered at bottom
  - System registration in systems vector
  - Player rendering in `render()` via Shape component
- Updated `CMakeLists.txt` with new source file
- Build verified with MinGW (zero errors)

## 2026-07-19 — Enemy Spawning System

### Summary
Added enemy spawning with a dedicated ECS system. Enemies are red rectangles that appear above the screen at regular intervals and move downward.

### Done
- Created branch `feature/enemy-spawning`
- Added components: `EnemyTag` (marker), `Health` (hit points)
- Added `EnemySpawnSystem`:
  - Timer-based spawning every 2 seconds
  - Random X position (clamped to screen bounds)
  - Spawns enemies above the top edge (Y = -height)
  - Enemies move downward at 150 px/s
- Added `MovementSystem`:
  - Updates position for all entities with Transform + Velocity
  - Skips player entity (handled by PlayerMovementSystem)
- Updated `Game` class:
  - Shared entity ID counter (`next_entity_id_`) to prevent ID collisions
  - Registered new components and systems
  - Enemy rendering in `render()` via EnemyTag
- Updated `CMakeLists.txt` with new source files
- **Bugfix**: Fixed entity ID collision — player and enemy used separate static counters both starting at 1, causing the first enemy to overwrite the player's components
- Build verified with MinGW (zero errors)

## 2026-07-20 — Bullet Shooting System

### Summary
Player can now shoot yellow square bullets with the Space key, with a cooldown of 250ms. Bullets are automatically removed when they fly off the top of the screen or after 2 seconds lifetime.

### Done
- Created branch `feature/bullet-shooting`
- Added components: `BulletTag` (marker), `Lifetime` (auto-removal timer)
- Modified `PlayerMovementSystem`:
  - Added shooting logic triggered by Space key
  - Cooldown timer (0.25s) prevents spam — max 4 shots/sec
  - Bullets spawn just above the player, moving upward at 500 px/s
  - Bullet size: 8×8 px, yellow color
- Added `BulletCleanupSystem`:
  - Removes bullets that exit the top of the screen (position.y + half-height < 0)
  - Decrements lifetime each frame and removes expired bullets
  - Cleans up all bullet components (Transform, Velocity, Shape, BulletTag, Lifetime)
- Updated `Game` class:
  - Registered BulletTag and Lifetime components
  - Added BulletCleanupSystem to systems list
  - Renders bullets in `render()` via BulletTag
- Updated `CMakeLists.txt` with new source file
- Build verified with MinGW (zero errors)

## 2026-07-20 — Collision System (Bullet vs Enemy)

### Summary
Added collision detection between bullets and enemies using SFML 3.0 AABB intersection. Bullets are destroyed on impact; enemies take damage (Health -1) and are destroyed when HP reaches 0. Uses deferred removal pattern to avoid iterator invalidation.

### Done
- Created branch `feature/collision-system`
- Added `CollisionSystem` (`src/systems/CollisionSystem.hpp` / `.cpp`):
  - Uses SFML 3.0 `getGlobalBounds()` + `findIntersection()` for AABB overlap — no manual bounds calculation
  - Two-phase update: phase 1 collects collision pairs, phase 2 applies removals (prevents iterator invalidation in unordered_map)
  - Bullet always destroyed on collision
  - Enemy takes damage (`Health -= 1`), fully removed when HP ≤ 0
  - Early break: one bullet can only hit one enemy per frame
  - Registered after MovementSystem, before BulletCleanupSystem (collision checked before off-screen cleanup)
- Updated `Game.cpp` — added `#include` and system registration
- Updated `CMakeLists.txt` — added `CollisionSystem.cpp`
- Enemy HP set to 2 for testing (two hits to kill)
- Build verified with MinGW (zero errors)

## Next
- PlayScene — ties all systems together, owns ECS world
- Scoring system
- Asset pipeline: textures, sounds
