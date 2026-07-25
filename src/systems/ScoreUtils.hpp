#ifndef SCORE_UTILS_HPP
#define SCORE_UTILS_HPP

#include "Config.hpp"

/// @brief Returns the updated score after destroying one enemy.
/// @param current_score The score before the kill.
/// @return The score after adding the kill points.
inline int award_kill_score(int current_score)
{
    return current_score + config::score::points_per_kill;
}

#endif // SCORE_UTILS_HPP