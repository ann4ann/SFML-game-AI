import os
import subprocess
import asyncio
from pathlib import Path
from mcp.server.fastmcp import FastMCP  # type: ignore

app = FastMCP("build-test")
PROJECT_ROOT = Path(__file__).parent.parent.parent

# Inherit parent environment and append MinGW binaries to PATH
MINGW_PATH = "C:/msys64/ucrt64/bin"
ENV = os.environ.copy()
ENV["PATH"] = f"{MINGW_PATH};{ENV.get('PATH', '')}"

async def run_command(cmd, cwd=None, timeout=300):
    """Run a command with timeout."""
    try:
        proc = await asyncio.create_subprocess_shell(
            cmd,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.STDOUT,
            stdin=asyncio.subprocess.DEVNULL,  # Disable interactive input
            cwd=cwd or str(PROJECT_ROOT),
            env=ENV
        )
        try:
            stdout, _ = await asyncio.wait_for(proc.communicate(), timeout=timeout)
            return stdout.decode().strip()
        except asyncio.TimeoutError:
            proc.kill()
            await proc.wait()
            return f"ERROR: Timeout after {timeout}s executing: {cmd}"
    except Exception as e:
        return f"ERROR: {e}"


@app.tool()
async def configure() -> str:
    """Configure CMake (Debug build)."""
    build_dir = PROJECT_ROOT / "build"
    build_dir.mkdir(exist_ok=True)
    output = await run_command(
        'cmake -B build -G "MinGW Makefiles" '
        '-DCMAKE_BUILD_TYPE=Debug '
        '-DCMAKE_PREFIX_PATH="C:/msys64/ucrt64" '
        '-DSFML_DIR="C:/msys64/ucrt64/lib/cmake/SFML"'
    )
    return output if output else "Configure successful."

@app.tool()
async def build() -> str:
    """Build the project."""
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
        # Detect real failures: "error" anywhere, or "tests failed" with non-zero count
        lower = out.lower()
        if "error" in lower or "tests failed" in lower and "0 tests failed" not in lower:
            results.append(f"{step} FAILED:\n{out}")
            return "\n\n".join(results)
        else:
            results.append(f"{step}: OK")
    return "\n".join(results)

if __name__ == "__main__":
    app.run(transport="stdio")
