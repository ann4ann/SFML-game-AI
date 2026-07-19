# Progress

## Completed
- [x] Project scaffolding (CMake + SFML 3.0 + basic main.cpp)
- [x] `.clinerules` created with development guidelines
- [x] Memory Bank initialized (productContext, activeContext, systemPatterns, progress)
- [x] Move source files to `src/` directory
- [x] Refactor CMakeLists.txt to use `src/` structure
- [x] Basic SFML window (800×600) with black background
- [x] Game class with main loop (`run()` → `process_events()` / `update()` / `render()`)
- [x] Project renamed to `space-shooter`
- [x] Branch `feature/basic-project-setup` created and committed
- [x] FPS counter in top-left corner (sf::Text, Roboto font, green, updated every second)
- [x] ECS core: entity.hpp, component.hpp, components.hpp, system.hpp
- [x] InputSystem — keyboard controls for player
- [x] PlayerMovementSystem — update positions
- [x] EnemySpawnSystem — spawn enemies from top
- [x] MovementSystem — move non-player entities by velocity

## In Progress
- [ ] BulletLifetimeSystem — remove expired bullets
- [ ] CollisionSystem — bullet-enemy interaction
- [ ] PlayScene — ties all systems together, owns ECS world
- [ ] Update Game to use PlayScene + GameState

## Backlog (planned features)
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

## Known Issues
- No issues yet — project is in initial setup phase.

## Notes
- SFML 3.0 uses new event API (`event->is<sf::Event::Closed>()`) — different from SFML 2.x
- All project text must be in English per `.clinerules` rules
- Assets should be requested via image-gen MCP server, not coded as placeholders
- Build-test MCP server must be run after every significant change