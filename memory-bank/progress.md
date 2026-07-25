# Progress
> **Purpose**:  Reflects global status and milestones

## In Progress
- [ ] Main menu scene

## Completed
- [x] Image-gen MCP server — Gen-API Flux-2, README
- [x] Player ship sprite — textured sprite (AI-generated player_ship.png)
- [x] Enemy drone sprite — Sprite ECS component, textured enemy (AI-generated enemy_drone.png)
- [x] Scrolling starfield background with infinite vertical scroll
- [x] Sound-gen MCP server — procedural audio (5 sound types), README
- [x] Sound effects integration — laser on shoot, hit on damage, explosion on destruction (enemy HP=2)
- [x] Explosion animation — 4-frame spritesheet, ExplosionAnim component, ExplosionAnimationSystem, magic numbers in config
- [x] Bullet sprite — textured sprite (AI-generated bullet_bolt.png)
- [x] Window icon — pixel-art icon, set via sf::Image + setIcon
- [x] Game Over state — enemy bottom-boundary + player-enemy collision, dark red nebula background, GAME OVER + final score display
- [x] Player lives system — 3 lives, heart icons in top-right, lose life on enemy collision, Game Over at 0
- [x] Test-build MCP server — configure, build, test and full_check
- [x] Catch2 test infrastructure — smoke test, BUILD_TESTING option
- [x] Unit tests for Movement, Collision, Score, Spawn systems — 14 tests, pure functions extracted

## Backlog
1. Main menu scene

## Known Issues
- Entity without all components silently skipped in render — no error feedback
- CollisionSystem O(n*m) per frame — acceptable for current scale
- Explosion sound may clip if enemies die in rapid succession (multiple overlapping sf::Sound instances)
