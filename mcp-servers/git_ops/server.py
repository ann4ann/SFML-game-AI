import os
from mcp.server.fastmcp import FastMCP  # type: ignore
from git import Repo, GitCommandError   # type: ignore

# Initialize the server
mcp = FastMCP("Local Git Server")


def get_repo(repo_path: str) -> Repo:
    """Helper function to load a git repository."""
    if not os.path.exists(os.path.join(repo_path, '.git')):
        raise ValueError(f"Directory {repo_path} is not a git repository")
    return Repo(repo_path)


@mcp.tool()
def status(repo_path: str) -> str:
    """Return the current repository status (git status)."""
    try:
        repo = get_repo(repo_path)
        return repo.git.status()
    except GitCommandError as e:
        return f"Error executing git status: {e}"
    except Exception as e:
        return f"Error: {e}"


@mcp.tool()
def create_branch(repo_path: str, name: str) -> str:
    """Create a new branch from the current HEAD and switch to it."""
    try:
        repo = get_repo(repo_path)
        repo.git.checkout('-b', name)
        return f"Successfully created and switched to branch '{name}'"
    except GitCommandError as e:
        return f"Error creating branch '{name}': {e}"
    except Exception as e:
        return f"Error: {e}"


@mcp.tool()
def commit(repo_path: str, message: str) -> str:
    """Stage all changes and commit with the given message."""
    try:
        repo = get_repo(repo_path)
        repo.git.add('.')
        repo.git.commit('-m', message)
        return f"Successfully committed: {message}"
    except GitCommandError as e:
        return f"Error executing commit: {e}"
    except Exception as e:
        return f"Error: {e}"


@mcp.tool()
def push(repo_path: str) -> str:
    """Push the current branch to origin."""
    try:
        repo = get_repo(repo_path)
        current_branch = repo.active_branch.name
        result = repo.git.push('origin', current_branch)
        return f"Successfully pushed to origin/{current_branch}:\n{result}"
    except GitCommandError as e:
        return f"Error executing push: {e}"
    except Exception as e:
        return f"Error: {e}"


if __name__ == "__main__":
    # Run via stdio (standard input/output for client communication)
    mcp.run(transport="stdio")