# Project Context — Space Shooter (SFML 3.0 / C++17)

> **Purpose**: Single reference for all project knowledge. Read this instead of source files when possible.
> **IMPORTANT**: Always read this file FIRST before reading any source files. Never read source files as "reading sources" when solving a feature — rely on this document instead.

## Build Commands
```PowerShell
# Configure (clean build)
Set-Location D:\Projects\VibeProjects\SFMLgame; Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue; $env:Path = "C:/msys64/ucrt64/bin;$env:Path"; cmake -B build -G "MinGW Makefiles" 2>&1

# Build
Set-Location D:\Projects\VibeProjects\SFMLgame; cmake --build build 2>&1
```
- **Executable**: `build/space-shooter.exe`
- **Compiler**: MinGW UCRT64 (`C:/msys64/ucrt64`)
- **CMake**: `CMakeLists.txt` — target `space-shooter`, C++17, SFML 3.0 (Graphics, Window, System, Audio, Network)

## Architecture: ECS-lite
```
process_events() → update(dt) → render()
```
- **Entity** — `EntityId` (uint32_t) wrapper, 0 = INVALID
- **ComponentManager** — type-erased `unordered_map<EntityId, T>` per component type; methods: `register_component<T>()`, `add_component<T>()`, `get_component<T>()`, `has_component<T>()`, `remove_component<T>()`, `get_entities_with_component<T>()`
- **System** — base class with `virtual void update(float dt) = 0`
- **Two-phase removal** — collect pending removals first, apply after iteration

## Components (src/ecs/Components.hpp)
| Component | Fields | Role |
|-----------|--------|------|
| `Transform` | `sf::Vector2f position` | World position |
| `Velocity` | `sf::Vector2f velocity` | Movement per second |
| `Sprite` | `shared_ptr<sf::Texture> texture`, `unique_ptr<sf::Sprite> sprite` | Textured visual (move-only) |
| `Shape` | `sf::RectangleShape rect` | Fallback visual / hitbox |
| `PlayerTag` | *(marker)* | Identifies player |
| `EnemyTag` | *(marker)* | Identifies enemies |
| `BulletTag` | *(marker)* | Identifies bullets |
| `Health` | `int hp` | Hit points |
| `Lifetime` | `float remaining` | Auto-removal timer |
| `ExplosionAnim` | `int current_frame, total_frames`, `float frame_timer, frame_duration`, `sf::Vector2i frame_size`, `bool remove_on_finish` | Frame-based animation |

## Systems (src/systems/)
| System | File | Responsibility |
|--------|------|----------------|
| `PlayerMovementSystem` | PlayerMovementSystem.{hpp,cpp} | Keyboard input (arrows+WASD), diagonal normalization, screen clamping, fire bullets (Space, 250ms cooldown) |
| `EnemySpawnSystem` | EnemySpawnSystem.{hpp,cpp} | Timer-based spawning (2s interval), random X, downward 150px/s |
| `MovementSystem` | MovementSystem.{hpp,cpp} | `position += velocity * dt` for non-player entities |
| `CollisionSystem` | CollisionSystem.{hpp,cpp} | AABB via SFML 3.0 `findIntersection()`, deferred removal, enemy HP=2, scoring +10/kill, spawns explosions |
| `BulletCleanupSystem` | BulletCleanupSystem.{hpp,cpp} | Removes bullets off-screen or expired (Lifetime) |
| `ExplosionAnimationSystem` | ExplosionAnimationSystem.{hpp,cpp} | Frame-based spritesheet animation, auto-remove on finish |

## Config (src/Config.hpp)
```
config::window    { width=800, height=600 }
config::player    { speed=300, size_x=96, size_y=96 }
config::bullet    { speed=500, size=8, sprite_size=16, cooldown=0.25, lifetime=2.0 }
config::enemy     { speed=150, size_x=64, size_y=64, spawn_interval=2.0, hp=2 }
config::score     { points_per_kill=10 }
config::background { scroll_speed=50 }
config::explosion { sprite_size=64, frame_count=4, frame_duration=0.1, frame_pixel=256 }
```

## Game Class (src/Game.hpp / Game.cpp)
- Owns `ComponentManager`, `vector<unique_ptr<System>>`, `EntityId next_entity_id_`
- Loads: font, 3 sound buffers, starfield texture, bullet/enemy/explosion textures
- `create_player()` — creates player entity with Transform, Velocity, PlayerTag, Shape
- `render()` — draws starfield, player, enemies, explosions, bullets, HUD (Score + FPS)
- **Texture fallback**: `use_texture_fallback_` flag → if texture fails to load, draws Shape rectangle instead

## Assets
```
assets/fonts/Roboto.ttf
assets/imgs/player_ship.png, enemy_drone.png, bullet_bolt.png, starfield.png, explosion_sheet.png
assets/sounds/player_laser.wav, enemy_hit.wav, enemy_explosion.wav
```

## Conventions
- **Naming**: CamelCase classes, snake_case functions/variables, snake_case files
- **Docs**: `///` Doxygen comments on all public headers
- **Pointers**: smart pointers (shared_ptr for shared textures, unique_ptr for owned resources)
- **Constants**: all in `Config.hpp` — never hardcode literals
- **Commits**: Conventional Commits (feat:, fix:, docs:, test:)
- **Cross-platform**: SFML abstracts; paths use `/`

## Git Workflow
- Branch before feature: `git_ops.create_branch(repo_path, name)`
- Commit at each logical step: `git_ops.commit(repo_path, message)`
- Push: `git_ops.push(repo_path)`
- Repo: `D:\Projects\VibeProjects\SFMLgame`

## MCP Servers
| Server | Tools | Notes |
|--------|-------|-------|
| git_ops | status, create_branch, commit, push | 2 params: repo_path, name/message |
| image-gen | generate_image | prompt, path, width(256), height(256) → assets/imgs/ |
| sound-gen | generate_sound | sound_type, save_path, duration_secs → assets/sounds/ |

## Memory Bank
- `activeContext.md` — current session (🔴🟡🟢 markers)
- `productContext.md` — stable project overview (do not change)
- `progress.md` — in-progress, completed, backlog, known issues
- `systemPatterns-core.md` — architecture overview + component table
- `systemPatterns-details.md` — detailed patterns (scenes, resources, collision, SFML API)
