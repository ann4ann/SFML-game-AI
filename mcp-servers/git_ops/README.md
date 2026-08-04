# 🔧 git-ops MCP Server

A Model Context Protocol (MCP) server that wraps common Git operations, allowing AI agents to manage repositories directly through MCP tools.

## Overview

- **Language**: Python
- **Framework**: [FastMCP](https://github.com/modelcontextprotocol/python-sdk) (MCP Python SDK)
- **Dependencies**: `mcp`, `GitPython`
- **Transport**: stdio

## Tools

| Tool | Description |
|------|-------------|
| `status` | Return the current repository status (`git status`) |
| `create_branch` | Create a new branch from the current HEAD and switch to it |
| `commit` | Stage all changes and commit with the given message |
| `push` | Push the current branch to origin |

## Usage

### 1. Install dependencies

```bash
cd mcp-servers
uv sync
```

### 2. Register the server

Add the server to your MCP client configuration (e.g. `cline_mcp_settings.json`):

```json
{
  "mcpServers": {
    "git_ops": {
      "command": "uv",
      "args": [
        "--directory",
        "D:/Projects/VibeProjects/SFMLgame/mcp-servers/",
        "run",
        "git_ops/server.py"
      ]
    }
  }
}
```

### 3. Call the tools

Each tool requires a `repo_path` parameter pointing to a local Git repository, e.g. `D:/Projects/VibeProjects/SFMLgame`.

```text
status(repo_path)                    → git status
create_branch(repo_path, name)       → create and switch to a new branch
commit(repo_path, message)           → stage all changes and commit
push(repo_path)                      → push current branch to origin
```

## Notes

- `commit` stages **all** changes (`git add .`) before committing.
- `push` pushes the currently active branch to `origin`.
- All tools return human-readable success/error messages.