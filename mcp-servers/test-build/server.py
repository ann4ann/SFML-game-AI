import subprocess
import asyncio
from pathlib import Path
from mcp.server.fastmcp import FastMCP  # type: ignore

app = FastMCP("build-test")
PROJECT_ROOT = Path(__file__).parent.parent.parent

async def run_command(cmd, cwd=None):
    """Run a command and return combined stdout/stderr."""
    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.STDOUT,
        cwd=cwd or str(PROJECT_ROOT)
    )
    stdout, _ = await proc.communicate()
    return stdout.decode().strip()

@app.tool()
async def configure() -> str:
    """Configure CMake (Debug build)."""
    build_dir = PROJECT_ROOT / "build"
    build_dir.mkdir(exist_ok=True)
    output = await run_command("cmake -B build -DCMAKE_BUILD_TYPE=Debug")
    return output if output else "Configure successful."

@app.tool()
async def build() -> str:
    """Build the project."""
    # output = await run_command("cmake --build build")
    output = await run_command("cmake --build build")
    return output if output else "Build successful."

@app.tool()
async def test() -> str:
    """Run tests via CTest."""
    output = await run_command("ctest --test-dir build --output-on-failure")
    return output if output else "All tests passed."

@app.tool()
async def full_check() -> str:
    """Run configure, build, and test in sequence. Return summary."""
    results = []
    for step, func in [("Configure", configure), ("Build", build), ("Test", test)]:
        out = await func()
        if "error" in out.lower() or "failed" in out.lower():
            results.append(f"{step} FAILED:\n{out}")
            return "\n\n".join(results)
        else:
            results.append(f"{step}: OK")
    return "\n".join(results)

if __name__ == "__main__":
    app.run(transport="stdio")
