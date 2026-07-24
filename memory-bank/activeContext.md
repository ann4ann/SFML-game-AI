# Active Context

## 🔴 CRITICAL
- **Current feature**: Bullet sprite instead shape — completed

## 🟡 ACTIVE DECISIONS
- **Asset pipeline**: Image-gen MCP for visual assets, Sound-gen MCP for audio assets
- **Sound-gen MCP**: 5 procedural sound types (laser, explosion, powerup, hit, pickup) — pure Python WAV generation


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
- **Completed**: Player, Enemy and Bullet sprites, Scrolling starfield background, Sound integration, Explosion animation

### Next Steps
1. Main menu / Game Over scenes
