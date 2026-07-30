# Progress
> **Purpose**:  Reflects global status and milestones

## In Progress
- [ ] Main menu scene

## Completed
- [x] README/AI_WORKFLOW now has Russian translations
- [x] Zigzag enemy type — sine-wave movement, Zigzag component, ZigzagSystem, serpentine sprite (AI-generated enemy_zigzag.png, 30% spawn chance)
- [x] Repository documentation overhaul — LICENSE, CHANGELOG, README, ARCHITECTURE, AI_WORKFLOW, .clinerules
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

## Backlog
1. Main menu scene
2. Windows CI matrix

## Known Issues
- Entity without all components silently skipped in render — no error feedback
- CollisionSystem O(n*m) per frame — acceptable for current scale
- Explosion sound may clip if enemies die in rapid succession (multiple overlapping sf::Sound instances)