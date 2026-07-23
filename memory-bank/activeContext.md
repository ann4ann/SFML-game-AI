# Active Context

## 🔴 CRITICAL
- **Current feature**: Scrolling starfield background — generate starfield texture + infinite vertical scroll

## 🟡 ACTIVE DECISIONS
- **Asset pipeline**: Use image-gen MCP for all visual assets (sprites, textures). Standard size 256×256 for game objects, 512×512 for background tile.
- **Prompt style**: English, detailed, with keywords "pixel art", "seamless", "tiling".
- **Save path**: Always `assets/imgs/<name>.png` with absolute path parameter.
- **Background scrolling**: Two sprites tiled vertically, scroll speed configurable in Config.hpp.

## 🟢 CONTEXT

### What's Working
- CMake project `space-shooter` with SFML 3.0, MinGW build
- Game loop: process_events → update(dt) → render, black 800×600 window
- ECS: ComponentManager, Entity, System
- **9 components**: Transform, Velocity, Sprite, Shape, PlayerTag, EnemyTag, BulletTag, Health, Lifetime
- **6 systems**: PlayerMovement, EnemySpawn, Movement, CollisionSystem (+scoring), BulletCleanup
- **Config.hpp** with 5 namespaces: `config::window`, `config::player`, `config::bullet`, `config::enemy`, `config::score`
- **Rendering**: PlayerTag (sprite), EnemyTag (sprite via Sprite component), BulletTag (yellow) via Shape::rect
- **HUD**: FPS (green, top-left), Score (white, below FPS)
- **Image-gen MCP**: server_ga.py (Gen-API Flux-2), README, fallback support
- **Completed**: Player sprite, Enemy sprite

### Next Steps
1. Sounds, 
2. effects and 
3. final visual polish