#ifndef CONFIG_HPP
#define CONFIG_HPP

/// @file Config.hpp
/// @brief Central configuration constants for game balance.
/// All gameplay-related numeric values are defined here for easy tuning.

namespace config {

/// @brief Window dimensions.
namespace window {
    constexpr unsigned int width  = 800;
    constexpr unsigned int height = 600;
} // namespace window

/// @brief Player settings.
namespace player {
    constexpr float speed      = 300.0f;  ///< Movement speed (px/s)
    constexpr float size_x     = 96.0f;   ///< Player width  (px) — fits 256×256 sprite scaled down
    constexpr float size_y     = 96.0f;   ///< Player height (px)
} // namespace player

/// @brief Bullet settings.
namespace bullet {
    constexpr float speed      = 500.0f;  ///< Upward speed (px/s)
    constexpr float size       = 8.0f;    ///< Square side length (px)
    constexpr float cooldown   = 0.25f;   ///< Min time between shots (s) → 4 shots/s
    constexpr float lifetime   = 2.0f;    ///< Max lifetime before auto-removal (s)
} // namespace bullet

/// @brief Enemy settings.
namespace enemy {
    constexpr float speed          = 150.0f;  ///< Downward speed (px/s)
    constexpr float size_x         = 64.0f;   ///< Sprite width  (px)
    constexpr float size_y         = 64.0f;   ///< Sprite height (px)
    constexpr float spawn_interval = 2.0f;    ///< Seconds between spawns
    constexpr int   hp             = 1;       ///< Hit points
} // namespace enemy

/// @brief Score settings.
namespace score {
    constexpr int points_per_kill = 10;  ///< Points awarded per enemy destroyed
} // namespace score

} // namespace config

#endif // CONFIG_HPP