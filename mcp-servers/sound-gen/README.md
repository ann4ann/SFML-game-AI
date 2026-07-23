# Procedural Audio Generator — MCP Server

Generates short game sound effects procedurally (pure Python, no external audio libraries) and saves them as WAV files.

## Tool

### `generate_sound`

| Parameter       | Type    | Required | Description |
|----------------|---------|----------|-------------|
| `sound_type`   | string  | yes      | One of: `"laser"`, `"explosion"`, `"powerup"`, `"hit"`, `"pickup"` |
| `save_path`    | string  | yes      | Absolute filesystem path for the output `.wav` file (e.g. `D:/path/to/assets/sounds/player_laser.wav`) |
| `duration_secs`| float   | no       | Override duration in seconds. Falls back to a sensible default per type if omitted. |

**Returns:** `"Success"` on success, or an error string on failure.

## Sound Types

| Type        | Technique | Character | Default duration |
|-------------|-----------|-----------|------------------|
| `laser`     | Frequency sweep 400→4800 Hz + envelope | Sharp rising beep | 0.3 s |
| `explosion` | White noise burst + low rumble 60 Hz | Boom / rumble | 1.0 s |
| `powerup`   | Ascending arpeggio C5→E5→G5→C6 | Bright chime | 0.5 s |
| `hit`       | Noise burst + low thud 120 Hz | Short impact | 0.2 s |
| `pickup`    | Two-tone chime C5→E5 (major 3rd) | Coin / collect | 0.3 s |

## Output Format

- **Sample rate:** 44100 Hz
- **Bit depth:** 16-bit signed PCM
- **Channels:** Mono
- **Container:** WAV (RIFF)

## Usage Example

```python
# Via MCP protocol (called by the system)
generate_sound(
    sound_type="laser",
    save_path="D:/Projects/SFMLgame/assets/sounds/player_laser.wav",
    duration_secs=0.5
)
```

## Determinism

- `laser`, `powerup`, `hit`, `pickup` — **deterministic** (identical output for identical parameters).
- `explosion` — **non-deterministic** (uses `random.uniform()` for white noise; each call produces a unique burst).

## Dependencies

- Python 3.10+
- `mcp` (FastMCP)
- No external audio libraries.