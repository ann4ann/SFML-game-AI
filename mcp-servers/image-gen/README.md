# Image Generation MCP Server

An MCP (Model Context Protocol) server that provides text-to-image generation capabilities using the [Gen-API](https://gen-api.ru) service (Flux-2 model).

## Purpose

This server is used by the SFML game project to generate visual assets (sprites, textures, backgrounds) on demand. It exposes a single tool `generate_image` that can be called by the AI assistant to create game graphics.

## Setup

### Prerequisites

- Python 3.10+
- [uv](https://docs.astral.sh/uv/) package manager

### Environment Variables

Create a `.env` file in this directory with:

```
GEN_API_TOKEN=your_api_token_here
```

Get a token from [Gen-API](https://gen-api.ru).

### Installation

Dependencies are managed via `pyproject.toml` at the workspace root. Run:

```bash
uv sync
```

## Tool: `generate_image`

Generates an image from a text prompt and saves it to disk.

### Parameters

| Parameter | Type   | Required | Default | Description |
|-----------|--------|----------|---------|-------------|
| `prompt`  | string | yes      | —       | Text description of the image (e.g. "pixel art spaceship, top-down view") |
| `path`    | string | yes      | —       | Absolute filesystem path to save the image (e.g. `D:/Projects/SFMLgame/assets/imgs/player.png`) |
| `width`   | int    | no       | 256     | Image width in pixels |
| `height`  | int    | no       | 256     | Image height in pixels |

### Returns

- `"Success"` — image was generated and saved successfully.
- Error string — if generation or download failed.

### Workflow

1. Sends a generation request to Gen-API with the prompt and dimensions.
2. Polls the API every ~20 seconds (up to 15 retries) until the image is ready.
3. Downloads the resulting image and writes it to the specified `path`.

## Usage in the Project

When the AI assistant needs a game asset:

1. Determine the filename from the task context (or ask the user).
2. Call `generate_image` with a detailed English prompt including style keywords like `"pixel art"`, `"top-down view"`, `"transparent background preferred"`.
3. Use the standard size of 256×256 for game objects.
4. Save to `assets/imgs/<filename>.png` using the absolute path.
5. Verify the file exists before referencing it in code.

## Files

- `server_ga.py` — Main server implementation using Gen-API.
- `server_hf.py` — Alternative implementation using Hugging Face (not active by default).
- `.env` — API token (not committed to git).
- `README.md` — This file.