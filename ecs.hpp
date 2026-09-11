#pragma once
#include "deps.hpp"

// https://austinmorlan.com/posts/entity_component_system/

using Index  = std::uint32_t;
using Entity = std::uint32_t;
using ComponentType = std::uint32_t;

const std::uint32_t MAX_ENTITIES = UINT32_MAX;

class IComponentStore
{
    public:
        virtual ~IComponentStore();
        virtual void entity_destroyed(Entity entity);
};

template <typename T>
class ComponentStore : IComponentStore
{
    private:
        std::unordered_map<Entity, Index> entity_component_indices;
        std::vector<T> components;
        std::vector<Index> unused_indices;

        void add_component_inner(Entity entity, T component);

    public:
        ComponentStore () = default;
        ~ComponentStore() = default;
        void entity_destroyed(Entity entity); // If entity doesn't exist, does nothing;

		bool has_component(Entity entity);
        T*   get_component(Entity entity); // If component exists, overwrite
        void add_component(Entity entity, const T& component); // If component exists, overwrite
        void add_component(Entity entity, T&& component); // If component exists, overwrite
};

template <ComponentType max_components>
class ComponentManager
{
    private:
        ComponentType curr_component_type = 0;
        // The size_t's in question are internal type ids;
        std::unordered_map<size_t, std::unique_ptr<IComponentStore>> stores;

        template <typename T>
        ComponentStore<T>& get_correct_store();

    public:
        ComponentManager () = default;
        ~ComponentManager() = default;
        void entity_destroyed(Entity entity); // If entity doesn't exist, does nothing;

        template <typename T>
        void register_component();
        template <typename T>
		bool has_component(Entity entity);
        template <typename T>
        T* get_component(Entity entity);
        template <typename T>
		void add_component(Entity entity, const T& component); // If component exists, overwrite
        template <typename T>
		void add_component(Entity entity, T&& component); // If component exists, overwrite
		template <typename T>
		bool component_type_registered();
};

template <ComponentType max_components>
class ECS
{
	private:
    	using Signature = std::bitset<max_components>;

		Entity curr_entity = 0;
    	std::unordered_map<Entity, Index> entity_signature_arr_indices;
    	std::vector<Signature> signatures; // An entity is an index into a signature array.
    	std::vector<Entity> unused_entities;
    	std::vector<Index>  unused_indices ;

    	ComponentManager<max_components> component_manager;

    	Signature& empty_signature();
    	Signature& get_signature(Entity entity);

    public:
        ECS () = default;
        ~ECS() = default;

        Entity create_entity();
        void destroy_entity (Entity entity);
        std::vector<Entity>& get_all_entities();
        template <typename T>
        std::vector<Entity>& get_entities_with_component();

		// Component operations
        template <typename T>
        void add_component   (Entity entity, T component);
        template <typename T>
        void remove_component(Entity entity);
        template <typename T>
        bool has_component   (Entity entity);
        template <typename T>
        T& get_component     (Entity entity);
};
