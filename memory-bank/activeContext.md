# Active Context

## Current Focus
Initial project setup and Memory Bank initialization.

## What's Working
- Basic CMake configuration with SFML 3.0 (Graphics, Window, System, Audio, Network)
- Minimal main.cpp that opens a window (800×600) with green background and a cyan rectangle
- CMake + CTest integration is ready

## Active Decisions
- Project structure layout follows `.clinerules`: src/, assets/, memory-bank/, mcp-servers/, .github/workflows/
- Game logic will be separated from rendering using an ECS-like design
- All text in project must be in English
- Source files will be moved from root to src/ as the project grows

## Next Steps
1. Move main.cpp and future source files into src/ directory
2. Update CMakeLists.txt to reflect src/ structure
3. Implement basic Entity-Component system skeleton
4. Add player ship entity with movement controls
5. Set up asset pipeline via image-gen MCP server
6. Add testing infrastructure

## Current Considerations
- SFML 3.0 API differs from SFML 2.x (new event system, sf::Vector2f changes)
- Must ensure cross-platform compatibility from the start
- Need to plan ECS architecture before implementing core game logic
- Memory Bank must never be deleted (per project rules)