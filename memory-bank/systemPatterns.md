# System Patterns

## Architecture Overview
The project follows an **ECS-like (Entity-Component-System) design** to separate game logic from rendering, as specified in `.clinerules`.

## Architectural Patterns

### 1. Entity-Component System (ECS-like)
- **Entities** — lightweight IDs representing game objects (player, enemies, bullets, etc.)
- **Components** — plain data structs (Position, Velocity, Sprite, Health, etc.)
- **Systems** — functions/classes that operate on entities with specific component combinations
- **Separation of concerns**: game logic systems never call SFML directly; rendering systems handle all draw calls

### 2. Game Loop Structure
```
ProcessInput() -> Update(dt) -> Render()
```
- Input handling is decoupled from game logic
- `Update()` uses fixed or variable time step
- `Render()` is the only place where SFML draw calls happen

### 3. Resource Management
- All assets (textures, sounds, fonts) are loaded via a central ResourceManager
- Smart pointers (`std::shared_ptr`, `std::unique_ptr`) used instead of raw pointers
- No global state — resources are passed to systems that need them

### 4. Scene/State Management
- A simple state machine for game screens: Menu, Playing, Paused, GameOver
- Each state owns its own ECS world and systems
- Transitions are explicit and managed by a SceneManager

### 5. Detailed ECS-lite Design (approved)

#### Components (`src/ecs/components.hpp`)
| Component | Fields | Description |
|-----------|--------|-------------|
| `Position` | `float x, y` | World position in pixels |
| `Velocity` | `float vx, vy` | Speed in pixels/second |
| `Sprite` | `sf::Sprite sprite` | SFML drawable sprite |
| `Collider` | `float radius` | Circle collision radius |
| `Health` | `int hp` | Hit points (<= 0 → entity removed) |
| `Lifetime` | `float remaining` | Seconds until auto-removal (bullets) |
| `PlayerInput` | *(marker)* | Identifies player entity |
| `EnemyTag` | *(marker)* | Identifies enemy entities |
| `BulletTag` | *(marker)* | Identifies bullet entities |

#### Systems (`src/systems/`)
1. **InputSystem** — reads keyboard (arrows for movement, Space to fire) → sets Velocity on Player, spawns Bullets
2. **EnemySpawnSystem** — timer-based spawning of enemies at random X above screen
3. **BulletLifetimeSystem** — decrements Lifetime::remaining by dt, removes expired bullets
4. **MovementSystem** — `Position += Velocity * dt` for all entities with Position+Velocity
5. **CollisionSystem** — circle-vs-circle check between all (Bullet, Enemy) pairs; removes bullet, damages enemy
6. **RenderSystem** — iterates all entities with Sprite component, calls `window.draw()`

#### Scene/State Management
- `enum class GameState { Playing, Paused, GameOver }` in Game.hpp
- PlayScene class owns ECS world (EntityManager + ComponentManager) and a vector of System pointers
- Game::update() and Game::render() delegate to current scene's update/render based on state

#### Collision Detection (Bullet vs Enemy)
- Each Collider has a `radius`
- CollisionSystem iterates all pairs (entity with BulletTag, entity with EnemyTag)
- Distance check: `dx² + dy² < (r1 + r2)²`
- On collision: decrement enemy Health, destroy bullet; if enemy Health <= 0 → destroy enemy

## Naming Conventions
- **Classes**: `CamelCase` (e.g., `PlayerShip`, `ResourceManager`)
- **Functions/Variables**: `snake_case` (e.g., `update_physics()`, `player_health`)
- **Files**: `snake_case` (e.g., `player_ship.hpp`, `resource_manager.cpp`)
- **Namespaces**: `snake_case` (e.g., `ecs::`, `render::`)

## Documentation
- Every public class and function in header files has `///` Doxygen-style comments
- Inline comments for non-obvious logic in `.cpp` files

## Cross-Platform Strategy
- SFML 3.0 abstracts windowing, input, and graphics
- File paths use `/` (works on all platforms)
- No platform-specific `#ifdef` unless absolutely necessary
- CMake handles platform detection and library linking

## Testing Strategy
- Unit tests via CTest (integrated in CMakeLists.txt)
- Tests live alongside source code in `src/` or a dedicated `tests/` directory
- Build-test MCP server runs CMake + CTest after every significant change