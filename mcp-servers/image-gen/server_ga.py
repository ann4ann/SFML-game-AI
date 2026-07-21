import os
from pathlib import Path
import json
import time

import requests # type: ignore
from mcp.server.fastmcp import FastMCP, Image  # type: ignore
from dotenv import load_dotenv  # type: ignore

# Token
env_path = Path(__file__).parent / ".env"
load_dotenv(dotenv_path=env_path)

GEN_API_TOKEN = os.getenv("GEN_API_TOKEN")
if not GEN_API_TOKEN:
    raise ValueError("The environment variable GEN_API_TOKEN is not set.")

BASE_URL = "https://api.gen-api.ru/api/v1"
DEFAULT_MODEL = "flux-2"

# MCP server
mcp = FastMCP("Gen-API Image Generator")


def _req_for_generation(prompt: str, width: int = 256, height: int = 256) -> str | int:
    """
    Send a text-to-image generation request to the Gen-API.

    Args:
        prompt: Text description of the image to generate.
        width: Image width in pixels (default 256).
        height: Image height in pixels (default 256).

    Returns:
        Request ID (int) on success, or a raw error string on failure.
    """
    url = f"{BASE_URL}/networks/{DEFAULT_MODEL}"

    payload = json.dumps({
        "model": "standard",
        "prompt": prompt,
        "width": width,
        "height": height
    })
    headers = {
        'Authorization': f'Bearer {GEN_API_TOKEN}',
        'Content-Type': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)

    data = response.json()

    return data.get("request_id") if "request_id" in data else str(data)


def _get_image_url_by_id(id: int) -> str:
    """
    Poll the Gen-API for a completed generation result and retrieve the image URL.

    Args:
        id: The request ID returned by _req_for_generation.

    Returns:
        Image URL string on success, "waiting" if generation is still in progress,
        or a raw error string on failure.
    """
    url = f"https://api.gen-api.ru/api/v1/request/get/{id}"

    payload = {}
    headers = {
        'Authorization': f'Bearer {GEN_API_TOKEN}'
    }

    response = requests.request("GET", url, headers=headers, data=payload)

    data = response.json()
    if "status" not in data:
        return str(data)

    return data.get("result")[0] if data["status"] == "success" else "waiting"


def _save_image_by_url(url: str, save_path: str) -> str:
    """
    Download an image from a URL and save it to the local filesystem.

    Args:
        url: The URL of the generated image.
        save_path: Absolute filesystem path where the image will be saved.

    Returns:
        "Success" on successful download, or an error string on failure.
    """
    response = requests.get(url)
    if response.status_code == 200:
        with open(save_path, 'wb') as file:
            file.write(response.content)
        return "Success"
    return f"Error: {response.status_code}"


# Tools
@mcp.tool()
def generate_image(prompt: str, path: str, width: int = 256, height: int = 256) -> str:
    """
    Generate an image using the Gen-API (Flux-2 model) and save it to disk.

    This is the main MCP tool exposed to the system. It submits a generation
    request, polls for completion, downloads the result, and saves it to the
    specified path. A retry loop handles the waiting period.

    Args:
        prompt: Text description of the image to generate (e.g. "pixel art spaceship").
        path: Absolute filesystem path to save the generated image (e.g. "D:/path/to/player.png").
        width: Image width in pixels (default 256).
        height: Image height in pixels (default 256).

    Returns:
        "Success" on successful generation and save, or an error string on failure.
    """
    gen_id = _req_for_generation(prompt, width, height)
    time.sleep(5)
    if not isinstance(gen_id, int):
        return gen_id

    retry_count = 15
    delay = 15
    for i in range(retry_count):
        res = _get_image_url_by_id(gen_id)
        time.sleep(5)
        if res == "waiting":
            time.sleep(delay)
            continue
        if not res.startswith("https:"):
            return "Error"
        # get img from link
        _save_image_by_url(res, path)
        time.sleep(5)
        break

    return "Success"


# Run MCP-server
if __name__ == "__main__":
    mcp.run(transport="stdio")