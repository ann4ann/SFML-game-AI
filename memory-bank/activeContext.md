# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Game Over state — enemy reaches bottom or collides with player → game stops, displays GAME OVER + final score
- **Branch**: `feature/game-over`

## 🟡 ACTIVE DECISIONS
- **GameOverSystem**: new ECS system in `src/systems/`, checks enemy bottom-edge + player-enemy AABB collision, sets `bool* game_over` flag
- **Game state**: `bool game_over_` in Game; `update()` skips when set; `render()` draws game over screen (dark bg + text)
- **Background asset**: `assets/imgs/game_over_bg.png` (512×512, generated via image-gen MCP)
- **Config**: add `config::game_over` namespace (font sizes, text offsets)

## 🟢 CONTEXT
- Shape component has centered origin (`setOrigin(size/2)`) — AABB must account for this
- Player-enemy collision NOT currently implemented — adding in GameOverSystem
- GameOverSystem registered last (after ExplosionAnimationSystem)
