# System Patterns — Details

## Architectural Patterns (Non-Critical)

### Scene/State Management (planned)
- `enum class GameState { Playing, Paused, GameOver }` in Game.hpp
- Each state owns its own ECS world and systems
- Transitions managed by a SceneManager or Game class

### Resource Management (planned)
- Assets (textures, sounds, fonts) loaded via central ResourceManager
- Smart pointers (`std::shared_ptr`, `std::unique_ptr`) instead of raw pointers
- No global state — resources passed to systems that need them

## Testing Strategy
- Unit tests via CTest (integrated in CMakeLists.txt)
- Tests in `src/` or dedicated `tests/` directory
- Build-test MCP server runs CMake + CTest after every significant change

## Cross-Platform
- SFML 3.0 abstracts windowing, input, graphics
- File paths use `/` (works on all platforms)
- No platform-specific `#ifdef` unless absolutely necessary
- CMake handles platform detection and library linking
- Build verified with MinGW (UCRT64) on Windows

## Collision Detection Detail
- **Method**: AABB overlap via SFML 3.0 `sf::FloatRect::findIntersection()`
- **Shape bounds**: `shape->rect.setPosition(transform->position)` then `rect.getGlobalBounds()`
- **Two-phase (deferred removal)**:
  1. Snapshot bullet and enemy ID vectors; iterate pairs, record hits in `pendingRemovals`
  2. Iterate `pendingRemovals` safely, remove components
- **Bullet**: always destroyed on collision
- **Enemy**: `Health -= 1`; removed when HP ≤ 0

## SFML 3.0 API Notes
- New event API: `event->is<sf::Event::Closed>()` instead of `event.type == sf::Event::Closed`
- `sf::VideoMode` uses braced initializer: `sf::VideoMode({width, height})`
- `sf::FloatRect::findIntersection()` returns `std::optional<sf::FloatRect>` — use `.has_value()` to check overlap