#ifndef COLLISION_UTILS_HPP
#define COLLISION_UTILS_HPP

#include <SFML/Graphics.hpp>
#include <optional>

/// @brief Checks if two SFML FloatRects overlap (AABB intersection).
/// @param a First rectangle.
/// @param b Second rectangle.
/// @return true if the two rectangles intersect.
inline bool aabb_overlap(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.findIntersection(b).has_value();
}

#endif // COLLISION_UTILS_HPP