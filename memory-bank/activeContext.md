# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Player lives system (expanded) — enemy bottom breach also costs a life
- **Branch**: `feature/lives-system`

## 🟡 ACTIVE DECISIONS
- Both player-enemy collision AND enemy bottom breach decrement lives
- Enemy is removed on bottom breach (same as collision)
- Game Over triggers only when lives reach 0
- Heart HUD renders in top-right corner

## 🟢 CONTEXT
- Heart sprite: `assets/imgs/heart.png` (256×256 pixel art, generated via image-gen MCP)
- GameOverSystem: remove_enemy_components() added for clean enemy removal
- Lives system: 3 lives, config::lives namespace
