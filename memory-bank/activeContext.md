# Active Context

## 🔴 CRITICAL
- **Current feature**: Explosion animation — completed

## 🟡 ACTIVE DECISIONS
- **Asset pipeline**: Image-gen MCP for visual assets, Sound-gen MCP for audio assets
- **Sound-gen MCP**: 5 procedural sound types (laser, explosion, powerup, hit, pickup) — pure Python WAV generation
- **Save path pattern for sounds**: `assets/sounds/<filename>.wav` via absolute `save_path` parameter
- **Sound architecture**: `std::unique_ptr<sf::Sound>` stored in Game, raw pointers passed to systems (nullptr = silent fallback)
- **Explosion animation**: 4-frame spritesheet (1024×256), frame-based `ExplosionAnimationSystem`, spawned by CollisionSystem on enemy death

## 🟢 CONTEXT

### What's Working
- CMake project `space-shooter` with SFML 3.0, MinGW build
- Game loop: process_events → update(dt) → render, 800×600 window
- ECS: ComponentManager, Entity, System
- **10 components**: Transform, Velocity, Sprite, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime, ExplosionAnim
- **7 systems**: PlayerMovement, EnemySpawn, Movement, CollisionSystem (+scoring), BulletCleanup, ExplosionAnimationSystem
- **Config.hpp** with 7 namespaces: `config::window`, `config::player`, `config::bullet`, `config::enemy`, `config::score`, `config::background`, `config::explosion`
- **Rendering**: PlayerTag (sprite), EnemyTag (sprite via Sprite component), Explosion (sprite via Sprite + ExplosionAnim), BulletTag (yellow) via Shape::rect
- **HUD**: FPS (green, top-left), Score (white, below FPS)
- **Image-gen MCP**: server_ga.py (Gen-API Flux-2), README, fallback support
- **Sound-gen MCP**: server.py, README, 5 procedural sound types
- **Sound effects**: Laser on shoot (PlayerMovementSystem), hit on damage + explosion on destruction (CollisionSystem)
- **Visual effects**: Explosion animation on enemy kill via ExplosionAnimationSystem (frame-based, auto-remove)
- **Completed**: Player sprite, Enemy sprite, Scrolling starfield background, Sound-gen server, Sound integration, Explosion animation

### Next Steps
1. Main menu / Game Over scenes
