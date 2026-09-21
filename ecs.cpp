#include "ecs.hpp"

template <typename T>
void ComponentStore<T>::add_component_inner(Entity entity, T component)
{
    auto component_ptr = this->get_component(entity);
    if (component_ptr == nullptr)
    {
        if (this->unused_indices.size() == 0)
        {
            Index top_index = static_cast<Index>(this->components.size());
            this->components.push_back(std::move(component));
            this->entity_component_indices.insert({entity, top_index});
        }
        else
        {
            Index index = this->unused_indices.pop_back();
            this->components[index] = std::move(component);
            this->entity_component_indices.insert({entity, index});
        }
    }
    else
    {
        *component_ptr = component;
    }
}

template <typename T>
void ComponentStore<T>::entity_destroyed(Entity entity)
{
    auto iterator = this->entity_component_indices.find(entity);
    if (iterator != this->entity_component_indices.end())
    {
        Index index = this->entity_component_indices[entity];
        this->components[index] = { 0 };
        this->entity_component_indices.erase(entity);
        unused_indices.push_back(index);
    }
}

template <typename T>
bool ComponentStore<T>::has_component(Entity entity)
{
    auto iterator = this->entity_component_indices.find(entity);
    if (iterator != this->entity_component_indices.end())
    {
        return true;
    }
    return false;
}

template <typename T>
T* ComponentStore<T>::get_component(Entity entity)
{
    auto iterator = this->entity_component_indices.find(entity);
    if (iterator != this->entity_component_indices.end())
    {
        Index index = this->entity_component_indices[entity];
        return &this->components[index];
    }
    return nullptr;
}

template <typename T>
void ComponentStore<T>::add_component(Entity entity, const T& component)
{
    add_component_inner(entity, component);
}

template <typename T>
void ComponentStore<T>::add_component(Entity entity, T&& component)
{
    add_component_inner(entity, std::move(component));
}

template <ComponentType max_components>
template <typename T>
ComponentStore<T>& ComponentManager<max_components>::get_correct_store()
{
    std::size_t type_id = typeid(T).hash_code();
    assert(this->stores.find(type_id) != this->stores.end());
    ComponentStore<T> &store = static_cast<ComponentStore<T>>(this->stores[type_id]); // We know store is of type 'T'.
    return store;
}

template <ComponentType max_components>
void ComponentManager<max_components>::entity_destroyed(Entity entity)
{
    for (auto store : this->stores)
    {
        for (auto component : store.second())
        {
            component.entity_destroyed(entity);
        }
    }
}

template <ComponentType max_components>
template <typename T>
void ComponentManager<max_components>::register_component()
{
    assert(this->stores.size() < this->max_components);

    std::size_t type_id = typeid(T).hash_code();
    if (this->stores.find(type_id) == this->stores.end())
    {
        this->stores[type_id] = std::make_unique<ComponentStore<T>>();
    }
}

template <ComponentType max_components>
template <typename T>
bool ComponentManager<max_components>::has_component(Entity entity)
{
    ComponentStore<T>& store = this->get_correct_store<T>();
    return store.has_component(entity);
}

template <ComponentType max_components>
template <typename T>
T* ComponentManager<max_components>::get_component(Entity entity)
{
    ComponentStore<T>& store = this->get_correct_store<T>();
    return store.get_component(entity);
}

template <ComponentType max_components>
template <typename T>
void ComponentManager<max_components>::add_component(Entity entity, const T& component)
{
    ComponentStore<T>& store = this->get_correct_store<T>();
    store.add_component(entity, component);
}

template <ComponentType max_components>
template <typename T>
void ComponentManager<max_components>::add_component(Entity entity, T&& component)
{
    ComponentStore<T>& store = this->get_correct_store<T>();
    store.add_component(entity, std::move(component));
}

template <ComponentType max_components>
template <typename T>
bool component_type_registered()
{
    std::size_t type_id = typeid(T).hash_code();
    return this->stores.find(type_id) != this->stores.end();
}

template <ComponentType max_components>
Signature& empty_signature()
{
    return std::bitset<max_components>();
}

template <ComponentType max_components>
Signature& get_signature(Entity entity)
{
    assert(this->entity_signature_arr_indices.find(entity)
           != this->entity_signature_arr_indices.end());

	return this->signatures[this->entity_signature_arr_indices[entity]];
}

template <ComponentType max_components>
Entity ECS<max_components>::create_entity()
{
    Entity entity;

    if(this->unused_entities.size() == 0)
    {
        assert(curr_entity != MAX_ENTITIES);

		entity = this->curr_entity;
        ++this->curr_entity;
    }
    else
    {
        Entity entity = this->unused_entities.pop();
    }

	this->assign_signature(entity);
}

template <ComponentType max_components>
void ECS<max_components>::destroy_entity(Entity entity)
{
    assert(this->entity_signature_arr_indices.find(entity)
           != this->entity_signature_arr_indices.end());

	this->erase_signature(entity);
}

template <ComponentType max_components>
std::vector<Entity>& ECS<max_components>::get_all_entities()
{
    std::vector<Entity> entities;
    for (auto entity : this->entity_signature_arr_indices.first())
    {
        entities.push_back(entity);
    }

    return entities;
}

template <ComponentType max_components>
template <typename... T>
std::vector<Entity>& ECS<max_components>::get_entities_with_component_types();
{
    assert(this->component_manager.component_type_registered<T>());

    std::vector<Entity> entities;
    for (auto entity : this->entity_signature_arr_indices.first())
    {
        entities.push_back(entity);
    }

    return entities;
}

template <ComponentType max_components>
template <typename T>
void ECS<max_components>::add_component(Entity entity, T component);

template <ComponentType max_components>
template <typename T>
void ECS<max_components>::remove_component(Entity entity);

template <ComponentType max_components>
template <typename T>
bool ECS<max_components>::has_component(Entity entity);

template <ComponentType max_components>
template <typename T>
T& ECS<max_components>::get_component(Entity entity);
