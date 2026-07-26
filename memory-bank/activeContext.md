# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Optimize CI workflow — add `concurrency`, `paths-ignore`, remove `push` for main/master
- **Branch**: `feature/ci-optimization`

## 🟡 ACTIVE DECISIONS
- Simple plan chosen: `concurrency` + `paths-ignore` + remove `push` for main/master
- Duplicate builds on PR merge eliminated by removing `push` for main/master
- Non-code changes (docs, assets, etc.) excluded from triggering CI

## 🟢 CONTEXT
- test-build MCP server: configure + build + test = OK
- Ready to implement changes to `.github/workflows/build.yml`