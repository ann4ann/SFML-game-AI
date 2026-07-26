# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2026-07-26

### Added
- GitHub Actions CI workflow — push/PR triggers, manual dispatch, Ubuntu 26.04, SFML 3.0 via apt
- Unit tests for Movement, Collision, Score, Spawn systems — 14 tests, pure functions extracted
- Catch2 test infrastructure — smoke test, BUILD_TESTING option
- Test-build MCP server — configure, build, test and full_check tools
- Player lives system — 3 lives, heart icons in top-right, lose life on enemy collision, Game Over at 0
- Game Over state — enemy bottom-boundary + player-enemy collision, dark red nebula background, GAME OVER + final score display
- Window icon — pixel-art icon, set via sf::Image + setIcon
- Bullet sprite — textured sprite (AI-generated bullet_bolt.png)
- Explosion animation — 4-frame spritesheet, ExplosionAnim component, ExplosionAnimationSystem
- Sound effects integration — laser on shoot, hit on damage, explosion on destruction
- Sound-gen MCP server — procedural audio (5 sound types: laser, explosion, powerup, hit, pickup)
- Image-gen MCP server — Gen-API Flux-2 for procedural pixel-art generation
- Game sprites (AI-generated): player_ship.png, enemy_drone.png, starfield.png (seamless)
- Sprite ECS component (shared_ptr<sf::Texture> + unique_ptr<sf::Sprite>, move-only)
- Background infinite vertical scrolling tiles
- Bullet shooting system — Space key, 250ms cooldown, BulletCleanupSystem
- Collision system — AABB via SFML 3.0 findIntersection, two-phase removal, enemy HP=2
- Scoring system — +10 points per enemy kill
- Config.hpp with constexpr constants for all game balance values
- FPS counter (green, top-left), Roboto font
- ECS-lite core: ComponentManager, Entity, System, 6 components
- Player movement — keyboard (arrows + WASD), diagonal normalization, screen clamping
- Enemy spawning — timer-based (2s interval), random X, downward 150px/s
- Object-oriented CMake project with SFML 3.0 dependency
- Memory Bank methodology — project context, architecture docs, progress tracking
- MCP git-ops server — status, create_branch, commit, push tools
- .clinerules — AI agent configuration for vibe-coding workflow
- dev-journal.md — daily development log

### Fixed
- Entity ID collision — shared `next_entity_id_` in Game required (separate static counters caused overwrite)
- CI duplication — removed `push` for main/master branches, added paths-ignore
- Test-build MCP: os.environ.copy() required for PATH lookup