# Dev Journal

## 2026-07-18 - Project scaffolding + MCP git-ops
### Done
- Created CMake project, .clinerules, Memory Bank (4 files)
- Game class: window, game loop (process/update/render)
- Custom MCP git-ops server (status, create_branch, commit, push)

## 2026-07-19 - FPS counter, ECS core, player movement, enemy spawning
### Done
- FPS counter (green, top-left), Roboto font
- ECS-lite: ComponentManager, Entity, System, 4 components, PlayerMovementSystem
- EnemySpawnSystem (timer 2s, random X, downward 150px/s) + MovementSystem
### Issues / Lessons
- Entity ID collision: shared `next_entity_id_` in Game required (separate static counters caused overwrite)

## 2026-07-20 - Bullet shooting, collision system, scoring system, game balance config
### Done
- Player shoots yellow bullets (Space, 250ms cooldown), BulletCleanupSystem (off-screen + lifetime)
- CollisionSystem: AABB via SFML 3.0 findIntersection, deferred two-phase removal, enemy HP=2
- Scoring: +10 points/enemy, `int* score` pointer in CollisionSystem, white text below FPS
- Created `Config.hpp` with constexpr constants for player, bullet, enemy, score, window
- Removed all hardcoded magic numbers from Game.cpp, system files, main.cpp

### Why it matters / What's next
- All balance values centralized in one header for easy tuning
- Next: PlayScene refactor, asset pipeline

### Issues / Lessons
- Enemy HP was mis-typed as 2 in journal (actual value was 1 in code) — fixed now in Config.hpp

## 2026-07-21 - Image-gen MCP server + asset generation rules
### Done
- Created image-gen MCP server (Gen-API Flux-2), server_ga.py (Hugging Face) with docstrings
- Filled README.md with setup, tool docs, project usage guide
- Updated .clinerules: detailed asset generation rules (size, path, verification, fallback)

### Why it matters / What's next
- Asset pipeline foundation ready: AI can now generate game sprites on demand

## 2026-07-23 - Game sprites with AI generated Textures and Sounds
### Done
- Generated via image-gen MCP (pixel-art): 
    - player_ship.png (cat-themed, 256×256)
    - enemy_drone.png (mouse-themed, 256×256)
    - starfield.png (seamless dark-blue starfield, 512×512)
- Added Sprite ECS component (shared_ptr<sf::Texture> + unique_ptr<sf::Sprite>, move-only)
- Replaced player and enemy shapes textured sprite (use fallback)
- Background - infinite vertical scrolling tiles: 2 sprites high, one above the other
- Created sound-gen MCP server: procedural WAV generation (pure Python) with 5 sound types: laser, explosion, powerup, hit, pickup

## 2026-07-24 - Sound effects integration, Explosion animation on enemy destruction
### Done
- Generated 3 sounds via sound-gen MCP: player_laser.wav, enemy_hit.wav, enemy_explosion.wav
- Added three sound slots to Game class, PlayerMovementSystem on shooting and CollisionSystem on enemy hit and destruction 
- Improved MCP sound-gen generators: laser, hit, explosion
- Generated explosion spritesheet (1024×256, 4 frames) via image-gen MCP
- Added an explosion entity with ExplosionAnim + ExplosionAnimationSystem, and CollisionSystem now spawns it at the enemy’s position on death
- Added "magic numbers → Config.hpp" rule to .clinerules

### Why it matters / What's next
- Game now has audio and visual feedback for shooting, hitting, and destroying enemies
- Next: final visual polish, main menu / Game Over scenes
