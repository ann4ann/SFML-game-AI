# Active Context
> **Purpose**:  Context of the current task only

## 🔴 CRITICAL
- **Current task**: Token optimization setup — create PROJECT_CONTEXT.md, update .clinerules, update memory-bank

## 🟡 ACTIVE DECISIONS
- **Token optimization**: Read PROJECT_CONTEXT.md first; use search_files/list_code_definition_names; batch tool calls
- **New rules**: Reading limit 3 files/step; forbidden: dev-journal.md, memory-bank/*, mcp-servers/* (unless MCP task)
- **Split systemPatterns-core.md** → invariants (core) + currentSet (inventory); added currentSet rule to .clinerules
- **Cleaned systemPatterns-details.md** — removed obsolete or duplicate items, added details usage rule to .clinerules
- **Removed duplicates**: feature workflow now only in .clinerules (systemPatterns-core style); Testing Strategy moved to .clinerules; reference info moved to PROJECT_CONTEXT.md

## 🟢 CONTEXT
- PROJECT_CONTEXT.md created — single reference for all project knowledge
- .clinerules updated with "Token Optimization (CRITICAL)" section
- Next: user will give step-by-step tasks
