# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Add zigzag enemy type with sprite
- **Branch**: `feature/zigzag-enemy`

## 🟡 ACTIVE DECISIONS
- New `Zigzag` component stores oscillation params (elapsed, amplitude, frequency, base_x)
- New `ZigzagSystem` updates velocity.x via sine wave; MovementSystem applies it
- EnemySpawnSystem rolls 30% chance for zigzag vs regular enemy
- New sprite generated: `enemy_zigzag.png`

## 🟢 CONTEXT
- Config: `config::zigzag_enemy` namespace added
- Component: `Zigzag` struct added
- System: `ZigzagSystem` created
- EnemySpawnSystem: modified to spawn zigzag variant
- Game: texture loading, component registration, system registration
- Build verified