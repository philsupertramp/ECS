# ECS

> Highly influenced by [tsprojectsblog's](https://tsprojectsblog.wordpress.com/portfolio/entity-component-system/) tutorial on ECS'

# Examples

## Entities:
- Node
- SceneTree
- GameObject

## Components:
- Drawable
- Physics

Usage:
```c++
#include <ECS/ECS.h>

// create new component class
class MyComponentClass
: public ECS::Component<MyComponentClass>
{
    ...
};

// create new entity class
class MyEntityClass
: public ECS::Entity<MyEntityClass>
{
    ...
};

/* Creation */
// Create new entity
auto entityID = ECS::ECS_Engine->GetEntityManager()->CreateEntity<MyEntityClass>();
// add component to entity
auto component = ECS::ECS_Engine->GetComponentManager()->AddComponent<MyComponentClass>(entityID);

/* Management */
/**
 * Override 
 * - IEntity::OnEnable()
 * - IEntity::OnDisable()
 */
auto Entity = component->GetOwner();
Entity->SetActive(true); // -> Calls OnEnable()
Entity->SetActive(false); // -> Calls OnDisable()
component->SetActive(true);

/* Retrieval */
// retrieve entity obj
auto entity = ECS::ECS_Engine->GetEntityManager()->GetEntity(entityID);
// retrieve entity id from obj
auto sameEntityID = ECS::ECS_Engine->GetEntityManager()->GetEntityId(entity);
sameEntityID == entityID;
// retrieve component from obj
auto sameComponent = ECS::ECS_Engine->GetComponentManager()->GetComponent<MyComponentClass>(entityID);
component == sameComponent;

auto stillSameComponent = entity->GetComponent<MyComponentClass>();
component == stillSameComponent;

entity->AddComponent<MyComponentClass>();
entity->RemoveComponent<MyComponentClass>();
entity->AddComponent<MyComponentClass>();

/* Cleanup */
// remove specific component from entity
ECS::ECS_Engine->GetComponentManager()->RemoveComponent<MyComponentClass>(entityID);

// remove all components from entity
ECS::ECS_Engine->GetComponentManager()->RemoveAllComponents(entityID);

// remove entity
ECS::ECS_Engine->GetEntityManager()->DestroyEntity(entityID);
```

see `main.cpp` for more examples.
