# Active Context

## Current Focus
Implementing player sprite rendering (via image-gen MCP).

## What's Working
- CMake project `space-shooter` with SFML 3.0 (Graphics, Window, System, Audio, Network)
- `src/` directory structure with `main.cpp`, `Game.hpp`, `Game.cpp`
- Game class with main loop: black background window (800×600), close-event handling
- Successful build and link

## Active Decisions
- Project structure layout follows `.clinerules`: src/, assets/, memory-bank/, mcp-servers/, .github/workflows/
- Game logic will be separated from rendering using an ECS-like design
- All text in project must be in English
- Player sprite will be generated via image-gen MCP server (no placeholder code)

## Next Steps
1. Request player ship texture via image-gen MCP server
2. Add sprite rendering to the Game class (load texture, draw in render())
3. Implement basic Entity-Component system skeleton
4. Add player entity with movement controls
5. Set up asset pipeline for enemies and background
6. Add testing infrastructure

## Current Considerations
- SFML 3.0 API differs from SFML 2.x (new event system, sf::Vector2f changes)
- Must ensure cross-platform compatibility from the start
- Need to plan ECS architecture before implementing core game logic
- Memory Bank must never be deleted (per project rules)