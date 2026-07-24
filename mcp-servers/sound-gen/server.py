import math
import os
import random
import struct
from pathlib import Path

from mcp.server.fastmcp import FastMCP  # type: ignore

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------
SAMPLE_RATE = 44100
AMPLITUDE = 0.5           # master volume (0.0 – 1.0)
OUTPUT_DIR = Path("D:/Projects/VibeProjects/SFMLgame/assets/sounds")

# ---------------------------------------------------------------------------
# MCP server
# ---------------------------------------------------------------------------
mcp = FastMCP("Procedural Audio Generator")

# ---------------------------------------------------------------------------
# WAV writer (pure Python, no external libs)
# ---------------------------------------------------------------------------

def _write_wav(path: Path, samples: list[float], sample_rate: int = SAMPLE_RATE) -> None:
    """
    Write a list of float samples (range -1.0 … 1.0) as a 16-bit mono WAV file.
    """
    num_channels = 1
    bits_per_sample = 16
    byte_rate = sample_rate * num_channels * bits_per_sample // 8
    block_align = num_channels * bits_per_sample // 8
    data_size = len(samples) * block_align
    file_size = 36 + data_size

    with open(path, "wb") as f:
        # RIFF header
        f.write(b"RIFF")
        f.write(struct.pack("<I", file_size))
        f.write(b"WAVE")

        # fmt chunk
        f.write(b"fmt ")
        f.write(struct.pack("<I", 16))           # chunk size
        f.write(struct.pack("<H", 1))            # PCM format
        f.write(struct.pack("<H", num_channels))
        f.write(struct.pack("<I", sample_rate))
        f.write(struct.pack("<I", byte_rate))
        f.write(struct.pack("<H", block_align))
        f.write(struct.pack("<H", bits_per_sample))

        # data chunk
        f.write(b"data")
        f.write(struct.pack("<I", data_size))

        for s in samples:
            # Clamp and convert to 16-bit signed integer
            s = max(-1.0, min(1.0, s))
            packed = struct.pack("<h", int(s * 32767))
            f.write(packed)


# ---------------------------------------------------------------------------
# Sample generators
# ---------------------------------------------------------------------------

def _sine(sample_rate: int, freq: float, duration: float,
          amplitude: float = AMPLITUDE) -> list[float]:
    """Generate a pure sine wave."""
    n = int(sample_rate * duration)
    return [amplitude * math.sin(2.0 * math.pi * freq * t / sample_rate)
            for t in range(n)]


def _noise(sample_rate: int, duration: float,
           amplitude: float = AMPLITUDE) -> list[float]:
    """Generate white noise."""
    n = int(sample_rate * duration)
    return [amplitude * random.uniform(-1.0, 1.0) for _ in range(n)]


def _sweep(sample_rate: int, freq_start: float, freq_end: float,
           duration: float, amplitude: float = AMPLITUDE) -> list[float]:
    """
    Generate a frequency sweep (chirp) from *freq_start* to *freq_end*.
    Uses a linear frequency ramp.
    """
    n = int(sample_rate * duration)
    samples: list[float] = []
    for t in range(n):
        phase = t / sample_rate
        instantaneous_freq = freq_start + (freq_end - freq_start) * (phase / duration)
        samples.append(amplitude * math.sin(2.0 * math.pi * instantaneous_freq * phase))
    return samples


def _apply_envelope(samples: list[float],
                    attack: float = 0.01,
                    release: float = 0.05) -> list[float]:
    """Apply a simple attack/release amplitude envelope (in seconds)."""
    n = len(samples)
    a = int(n * attack)
    r = int(n * release)

    out = samples[:]
    for i in range(a):
        out[i] *= i / a
    for i in range(max(r, 1)):
        out[n - 1 - i] *= i / r
    return out


# ---------------------------------------------------------------------------
# Per-type sound generators
# ---------------------------------------------------------------------------

def _gen_laser(duration: float = 0.3) -> list[float]:
    """Sharp rising sweep — classic laser. Moderate volume."""
    sr = SAMPLE_RATE
    raw = _sweep(sr, 400.0, 4800.0, duration, amplitude=0.25)
    return _apply_envelope(raw, attack=0.01, release=0.05)


def _gen_explosion(duration: float = 1.5) -> list[float]:
    """Deep, powerful explosion — heavy rumble + bright crack + sub bass + low bang."""
    sr = SAMPLE_RATE
    # Initial bright noise burst (short, punchy) — the "crack"
    crack = _noise(sr, duration * 0.12, amplitude=0.9)
    # Low rumble (sine at 65 Hz) — main body
    rumble = _sine(sr, 65.0, duration * 0.85, amplitude=1.0)
    # Sub-bass thump (sine at 40 Hz) — adds weight
    sub = _sine(sr, 40.0, duration * 0.6, amplitude=0.85)
    # Low-frequency bang — a short sweep from 300 Hz → 60 Hz for impact
    bang = _sweep(sr, 300.0, 60.0, duration * 0.25, amplitude=1.0)
    # Combine
    combined: list[float] = []
    n_total = max(len(crack), len(rumble), len(sub), len(bang))
    for i in range(n_total):
        v = 0.0
        if i < len(crack):
            fade = 1.0 - (i / len(crack))
            v += crack[i] * fade
        if i < len(rumble):
            v += rumble[i]
        if i < len(sub):
            v += sub[i]
        if i < len(bang):
            fade = 1.0 - (i / len(bang))
            v += bang[i] * fade
        combined.append(v)
    return _apply_envelope(combined, attack=0.001, release=0.25)


def _gen_powerup(duration: float = 0.5) -> list[float]:
    """Ascending arpeggio: C5 → E5 → G5 → C6 (bright chime)."""
    sr = SAMPLE_RATE
    notes = [523.25, 659.25, 783.99, 1046.50]  # C5, E5, G5, C6
    note_len = duration / len(notes)
    samples: list[float] = []
    for freq in notes:
        note = _sine(sr, freq, note_len, amplitude=0.5)
        note = _apply_envelope(note, attack=0.02, release=0.1)
        samples.extend(note)
    return samples


def _gen_hit(duration: float = 0.15) -> list[float]:
    """Short, soft impact — light tap (quieter than explosion)."""
    sr = SAMPLE_RATE
    # Quick noise burst — low volume
    noise_part = _noise(sr, duration * 0.4, amplitude=0.25)
    # Soft thud at a higher frequency (less boomy than explosion)
    thud = _sine(sr, 200.0, duration, amplitude=0.2)
    combined: list[float] = []
    n_noise = len(noise_part)
    n_thud = len(thud)
    n_total = max(n_noise, n_thud)
    for i in range(n_total):
        v = 0.0
        if i < n_noise:
            v += noise_part[i]
        if i < n_thud:
            v += thud[i]
        combined.append(v)
    return _apply_envelope(combined, attack=0.002, release=0.05)


def _gen_pickup(duration: float = 0.3) -> list[float]:
    """Quick two-tone chime (major 3rd: C5 → E5)."""
    sr = SAMPLE_RATE
    notes = [523.25, 659.25]  # C5, E5
    note_len = duration / len(notes)
    samples: list[float] = []
    for freq in notes:
        note = _sine(sr, freq, note_len, amplitude=0.5)
        note = _apply_envelope(note, attack=0.01, release=0.08)
        samples.extend(note)
    return samples


# ---------------------------------------------------------------------------
# Dispatch table
# ---------------------------------------------------------------------------
_GENERATORS: dict[str, tuple] = {
    "laser":     (_gen_laser,     0.3),
    "explosion": (_gen_explosion, 1.5),
    "powerup":   (_gen_powerup,   0.5),
    "hit":       (_gen_hit,       0.15),
    "pickup":    (_gen_pickup,    0.3),
}

_VALID_TYPES = list(_GENERATORS.keys())


# ---------------------------------------------------------------------------
# MCP Tool
# ---------------------------------------------------------------------------

@mcp.tool()
def generate_sound(
    sound_type: str,
    save_path: str,
    duration_secs: float | None = None,
) -> str:
    """
    Generate a procedural sound effect and save it as a WAV file.

    Args:
        sound_type: Type of sound — one of "laser", "explosion", "powerup", "hit", "pickup".
        save_path:  Absolute filesystem path where the WAV file will be saved
                    (e.g. "D:/path/to/assets/sounds/player_laser.wav").
        duration_secs: Optional duration in seconds. If omitted, a sensible default is used
                       per sound type (0.3 for laser, 1.0 for explosion, etc.).

    Returns:
        "Success" on successful generation and save, or an error string on failure.
    """
    # --- validation ---
    sound_type = sound_type.strip().lower()
    if sound_type not in _VALID_TYPES:
        return (
            f"Invalid sound_type '{sound_type}'. "
            f"Valid options: {', '.join(_VALID_TYPES)}"
        )

    if not save_path or save_path.strip() == "":
        return "save_path must be a non-empty string."
    

    out_path = Path(save_path.strip())

    # --- generate ---
    # Ensure parent directory exists
    out_path.parent.mkdir(parents=True, exist_ok=True)

    try:
        gen_func, default_dur = _GENERATORS[sound_type]
        dur = duration_secs if duration_secs is not None else default_dur
        samples = gen_func(dur)
        _write_wav(out_path, samples)
    except Exception as exc:
        return f"Error generating sound: {exc}"

    # --- verify ---
    if not out_path.exists() or out_path.stat().st_size < 100:
        return f"Generated file is missing or too small: {out_path}"

    return "Success"


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    # Ensure output directory exists
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    mcp.run(transport="stdio")