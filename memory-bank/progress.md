# Progress

## Completed
- [x] Project scaffolding (CMake + SFML 3.0 + basic main.cpp)
- [x] `.clinerules` created with development guidelines
- [x] Memory Bank initialized (productContext, activeContext, systemPatterns, progress)

## In Progress
- [ ] Move source files to `src/` directory
- [ ] Refactor CMakeLists.txt to use `src/` structure

## Backlog (planned features)
- [ ] ECS core: Entity Manager, Component storage, System base
- [ ] Player entity with movement (keyboard input)
- [ ] Enemy spawning and basic AI
- [ ] Bullet/projectile system
- [ ] Collision detection
- [ ] Scoring system
- [ ] Asset pipeline: player ship texture (via image-gen MCP)
- [ ] Asset pipeline: enemy textures
- [ ] Asset pipeline: background / starfield
- [ ] Sound effects (procedural or via API)
- [ ] Main menu scene
- [ ] Game Over / Restart flow
- [ ] Multiple levels / difficulty scaling
- [ ] Boss battles
- [ ] Particle effects (explosions, thrust)
- [ ] CI workflow (GitHub Actions)
- [ ] Unit tests for core systems
- [ ] Packaging (CPack)

## Known Issues
- No issues yet — project is in initial setup phase.

## Notes
- SFML 3.0 uses new event API (`event->is<sf::Event::Closed>()`) — different from SFML 2.x
- All project text must be in English per `.clinerules` rules
- Assets should be requested via image-gen MCP server, not coded as placeholders
- Build-test MCP server must be run after every significant change