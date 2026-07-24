# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Player lives system — 3 lives, heart icons in top-right, lose life on enemy collision, Game Over at 0
- **Branch**: `feature/lives-system`

## 🟡 ACTIVE DECISIONS
- **Lives tracking**: `int lives_` in Game, initialized to `config::lives::max_lives` (3)
- **GameOverSystem**: add `int* lives` param; on player-enemy collision → decrement lives, remove enemy, set game_over if lives <= 0
- **Heart rendering**: load `assets/imgs/heart.png`, draw `lives_` sprites in top-right corner
- **Config**: add `config::lives` namespace (max_lives, heart_size, spacing, margins)
- **Bottom breach**: stays as immediate Game Over (separate condition)

## 🟢 CONTEXT
- GameOverSystem already handles player-enemy AABB collision and enemy bottom breach
- Shape component has centered origin — AABB must account for this
- Heart sprite: 256×256 pixel art, generated via image-gen MCP
- GameOverSystem registered last (after ExplosionAnimationSystem)
