# Active Context

## 🔴 CRITICAL
- **Current task**: Game balance config extracted into `Config.hpp`. All magic numbers replaced with constexpr constants (player, bullet, enemy, score, window). No behavioural changes — same values as before.
- **Key decision**: Single header-only `Config.hpp` with nested namespaces per domain. No runtime reload needed at this stage.

## 🟡 ACTIVE DECISIONS
- **Config.hpp location**: Root of `src/` so any `.cpp` can include it without relative path gymnastics.
- **constexpr over const**: Values are compile-time constants — no runtime overhead, inlined by the compiler.
- **No config file parsing**: Not justified for 5-10 values. If balance needs runtime tweaking later, we switch to JSON/ini.

## 🟢 CONTEXT

### What's Working
- CMake project `space-shooter` with SFML 3.0, MinGW build
- Game loop: process_events → update(dt) → render, black 800×600 window
- ECS: ComponentManager, Entity, System
- **8 components**: Transform, Velocity, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime
- **6 systems**: PlayerMovement, EnemySpawn, Movement, CollisionSystem (+scoring), BulletCleanup
- **Config.hpp** with 4 namespaces: `config::window`, `config::player`, `config::bullet`, `config::enemy`, `config::score`
- **Rendering**: PlayerTag (cyan), EnemyTag (red), BulletTag (yellow) via Shape::rect
- **HUD**: FPS (green, top-left), Score (white, below FPS)

### Next Steps
1. PlayScene — extract ECS from Game into a dedicated scene, prepare State Machine
2. Asset pipeline — textures (via image-gen MCP), sounds
