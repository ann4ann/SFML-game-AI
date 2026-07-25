# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Unit tests for key systems (Movement, Collision, Score, Spawn)
- **Branch**: `feature/unit-tests`

## 🟡 ACTIVE DECISIONS
- Refactored: extracted `aabb_overlap()` → `CollisionUtils.hpp`, `award_kill_score()` → `ScoreUtils.hpp`
- Created 4 test files: `test_movement.cpp`, `test_collision.cpp`, `test_score.cpp`, `test_spawn.cpp`
- Updated `tests/CMakeLists.txt` with new sources and SFML libs
- All 14 tests passing

## 🟢 CONTEXT
- test-build MCP server: configure + build + test = OK
- 14/14 tests passed across all 4 systems
- Feature complete — ready to commit and push