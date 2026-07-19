#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <cstddef>
#include <vector>
#include "Entity.hpp"

/// @brief Base class for all systems.
class System {
public:
    virtual ~System() = default;

    /// @brief Called once per frame to update logic.
    /// @param dt Delta time in seconds.
    virtual void update(float dt) = 0;

protected:
    System() = default;
};

#endif // SYSTEM_HPP