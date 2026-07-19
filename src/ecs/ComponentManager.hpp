#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include "Entity.hpp"
#include "Component.hpp"

/// @brief Manages component pools for all entity-component pairs.
class ComponentManager {
public:
    ComponentManager() = default;
    ~ComponentManager() = default;

    /// @brief Registers a component type (must be called before use).
    /// @tparam T The component type.
    template<typename T>
    void register_component()
    {
        auto id = component_type_id<T>();
        if (pools_.find(id) == pools_.end())
        {
            pools_[id] = std::make_unique<ComponentPool<T>>();
        }
    }

    /// @brief Adds a component to an entity.
    /// @tparam T The component type.
    /// @param entity The target entity.
    /// @param component The component data.
    template<typename T>
    void add_component(Entity entity, T component)
    {
        auto* pool = get_pool<T>();
        pool->components[entity.id] = std::move(component);
    }

    /// @brief Removes a component from an entity.
    /// @tparam T The component type.
    /// @param entity The target entity.
    template<typename T>
    void remove_component(Entity entity)
    {
        auto* pool = get_pool<T>();
        pool->components.erase(entity.id);
    }

    /// @brief Checks if an entity has a component.
    /// @tparam T The component type.
    /// @param entity The entity to check.
    /// @return true if the entity has the component.
    template<typename T>
    bool has_component(Entity entity) const
    {
        const auto* pool = get_pool<T>();
        return pool->components.find(entity.id) != pool->components.end();
    }

    /// @brief Gets a component from an entity.
    /// @tparam T The component type.
    /// @param entity The entity.
    /// @return Pointer to the component, or nullptr if not found.
    template<typename T>
    T* get_component(Entity entity)
    {
        auto* pool = get_pool<T>();
        auto it = pool->components.find(entity.id);
        if (it != pool->components.end())
            return &it->second;
        return nullptr;
    }

    /// @brief Gets all entities that have a specific component.
    /// @tparam T The component type.
    /// @return A vector of entity IDs.
    template<typename T>
    std::vector<EntityId> get_entities_with_component() const
    {
        const auto* pool = get_pool<T>();
        std::vector<EntityId> result;
        result.reserve(pool->components.size());
        for (const auto& [eid, _] : pool->components)
        {
            result.push_back(eid);
        }
        return result;
    }

private:
    /// @brief Type-erased base for component pools.
    struct ComponentPoolBase {
        virtual ~ComponentPoolBase() = default;
    };

    /// @brief Typed component pool.
    template<typename T>
    struct ComponentPool : ComponentPoolBase {
        std::unordered_map<EntityId, T> components;
    };

    std::unordered_map<ComponentTypeId, std::unique_ptr<ComponentPoolBase>> pools_;

    /// @brief Gets the typed pool (non-const). Asserts if not registered.
    template<typename T>
    ComponentPool<T>* get_pool()
    {
        auto id = component_type_id<T>();
        auto it = pools_.find(id);
        // Pool should be registered before use
        return static_cast<ComponentPool<T>*>(it->second.get());
    }

    /// @brief Gets the typed pool (const). Asserts if not registered.
    template<typename T>
    const ComponentPool<T>* get_pool() const
    {
        auto id = component_type_id<T>();
        auto it = pools_.find(id);
        return static_cast<const ComponentPool<T>*>(it->second.get());
    }
};

#endif // COMPONENT_MANAGER_HPP