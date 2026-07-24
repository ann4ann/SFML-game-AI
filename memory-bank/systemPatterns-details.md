# System Patterns — Details

> **Purpose**: Non-critical patterns and API notes. Use ONLY when explicitly asked or when it cannot be done without it. Not a general reading source.

## Scene/State Management (planned)
- `enum class GameState { Playing, Paused, GameOver }` — planned for Game.hpp
- Each state would own its own ECS world and systems
- Transitions managed by a SceneManager or Game class
- `src/scenes/` directory exists but is empty — not yet implemented

## Resource Management (partial)
- Smart pointers (`std::shared_ptr`, `std::unique_ptr`) already used throughout — no raw pointers
- No global state — resources passed to systems that need them (via constructor)
- Central `ResourceManager` — **not yet implemented**; assets loaded directly in `Game` constructor

## Cross-Platform
- SFML 3.0 abstracts windowing, input, graphics
- File paths use `/` (works on all platforms)
- No platform-specific `#ifdef` unless absolutely necessary
- CMake handles platform detection and library linking
- Build verified with MinGW (UCRT64) on Windows

## SFML 3.0 API Notes
- New event API: `event->is<sf::Event::Closed>()` instead of `event.type == Event::Closed`
- `sf::VideoMode` uses braced initializer: `sf::VideoMode({width, height})`
- `sf::FloatRect::findIntersection()` returns `std::optional<sf::FloatRect>` — use `.has_value()` to check overlap
