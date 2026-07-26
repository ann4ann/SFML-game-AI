# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: GitHub Actions CI workflow
- **Branch**: `feature/ci-github-actions`

## 🟡 ACTIVE DECISIONS
- Created `.github/workflows/build.yml` — triggers on push to stable branches, PR to all branches, manual dispatch
- Wrapped MinGW paths in `if(WIN32)` in CMakeLists.txt for cross-platform compatibility
- Runner: `ubuntu-26.04` (SFML 3.0 available via apt)
- Single Linux job for now; Windows CI deferred

## 🟢 CONTEXT
- test-build MCP server: configure + build + test = OK
- Feature complete — ready to commit and push