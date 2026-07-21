import os
from pathlib import Path
from mcp.server.fastmcp import FastMCP, Image  # type: ignore
from dotenv import load_dotenv  # type: ignore
from huggingface_hub import InferenceClient  # type: ignore

env_path = Path(__file__).parent / ".env"
load_dotenv(dotenv_path=env_path)

HF_TOKEN = os.getenv("HF_TOKEN")

# Initialize the MCP server
mcp = FastMCP("HF-Image-Generator")

MODEL_ID = "Qwen/Qwen-Image"
PROVIDER = "fal-ai"


@mcp.tool()
def generate_image_via_hf(prompt: str) -> Image:
    """Generates an image based on a text prompt via the Hugging Face Serverless API.

    Args:
        prompt: Description of the image in English.
    """
    if not HF_TOKEN:
        raise ValueError(
            "Critical error: Environment variable HF_TOKEN is not set!"
        )

    client = InferenceClient(provider=PROVIDER, api_key=HF_TOKEN)

    # output is a PIL.Image object
    pil_image = client.text_to_image(prompt, model=MODEL_ID)

    # Save the PIL Image to JPEG bytes
    import io

    buf = io.BytesIO()
    pil_image.save(buf, format="JPEG")
    image_bytes = buf.getvalue()

    return Image(data=image_bytes, format="jpeg")


if __name__ == "__main__":
    mcp.run()