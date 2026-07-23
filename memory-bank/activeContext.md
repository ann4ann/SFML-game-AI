# Active Context

## 🔴 CRITICAL
- **Current feature**: Enemy sprite — replace red rectangle with textured sprite.
- **Asset**: `enemy_drone.png` (256×256) to be generated via image-gen MCP, mouse-themed pixel art.

## 🟡 ACTIVE DECISIONS
- **Asset pipeline**: Use image-gen MCP for all visual assets (sprites, textures). Standard size 256×256 for game objects.
- **Prompt style**: English, detailed, with keywords "pixel art", "top-down view", "transparent background preferred".
- **Save path**: Always `assets/imgs/<name>.png` with absolute path parameter.
- **Enemy sprite**: New `Sprite` component for ECS, shared texture via `std::shared_ptr<sf::Texture>`, scaled to 64×64.

## 🟢 CONTEXT

### What's Working
- CMake project `space-shooter` with SFML 3.0, MinGW build
- Game loop: process_events → update(dt) → render, black 800×600 window
- ECS: ComponentManager, Entity, System
- **8 components**: Transform, Velocity, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime
- **6 systems**: PlayerMovement, EnemySpawn, Movement, CollisionSystem (+scoring), BulletCleanup
- **Config.hpp** with 4 namespaces: `config::window`, `config::player`, `config::bullet`, `config::enemy`, `config::score`
- **Rendering**: PlayerTag (sprite), EnemyTag (red rect → sprite), BulletTag (yellow) via Shape::rect
- **HUD**: FPS (green, top-left), Score (white, below FPS)
- **Image-gen MCP**: server_ga.py (Gen-API Flux-2), README, fallback support

### Next Steps
1. PlayScene — extract ECS from Game into a dedicated scene, prepare State Machine
2. Asset pipeline — generate real textures via image-gen MCP, integrate into rendering