#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <cstddef>
#include <typeindex>
#include <unordered_map>

/// @brief Unique identifier for a component type.
using ComponentTypeId = std::size_t;

/// @brief Returns a unique ID for each component type.
/// @tparam T The component type.
/// @return A unique std::size_t for type T.
template<typename T>
inline ComponentTypeId component_type_id() noexcept
{
    static const ComponentTypeId id = std::type_index(typeid(T)).hash_code();
    return id;
}

/// @brief Base class for all components.
struct Component {
    virtual ~Component() = default;
};

#endif // COMPONENT_HPP