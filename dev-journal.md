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

## Next
- Implement ECS core (Entity Manager, Component storage)
- Add player ship entity with movement
- Set up asset pipeline via image-gen MCP
