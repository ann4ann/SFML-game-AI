# Progress
> **Purpose**:  Reflects global status and milestones

## In Progress
- [ ] Main menu scene

## Completed
- [x] CI optimization — Fixed duplication of CI workflow launch for PR and merge, added `paths-ignore`
- [x] CI cycle test
- [x] GitHub Actions CI workflow — push/PR triggers, manual dispatch, Ubuntu 26.04, SFML 3.0 via apt
- [x] Unit tests for Movement, Collision, Score, Spawn systems — 14 tests, pure functions extracted
- [x] Catch2 test infrastructure — smoke test, BUILD_TESTING option
- [x] Test-build MCP server — configure, build, test and full_check
- [x] Player lives system — 3 lives, heart icons in top-right, lose life on enemy collision, Game Over at 0
- [x] Game Over state — enemy bottom-boundary + player-enemy collision, dark red nebula background, GAME OVER + final score display
- [x] Window icon — pixel-art icon, set via sf::Image + setIcon
- [x] Bullet sprite — textured sprite (AI-generated bullet_bolt.png)
- [x] Explosion animation — 4-frame spritesheet, ExplosionAnim component, ExplosionAnimationSystem, magic numbers in config
- [x] Sound effects integration — laser on shoot, hit on damage, explosion on destruction (enemy HP=2)
- [x] Sound-gen MCP server — procedural audio (5 sound types), README


## Backlog
1. Main menu scene
2. Windows CI matrix

## Known Issues
- Entity without all components silently skipped in render — no error feedback
- CollisionSystem O(n*m) per frame — acceptable for current scale
- Explosion sound may clip if enemies die in rapid succession (multiple overlapping sf::Sound instances)