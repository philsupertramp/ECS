//
// Created by phil on 01.10.20.
//

#include "../../include/ECS/entity/EntityManager.h"


namespace ECS {

    EntityManager::EntityManager() :
            m_PendingDestroyedEntities(1024),
            m_NumPendingDestroyedEntities(0)
    { }

    EntityManager::~EntityManager() {
        for (auto ec : this->m_EntityRegistry) {
            delete ec.second;
            ec.second = nullptr;
        }
    }

    void EntityManager::DestroyEntity(EntityId entityId) {
        if(entityId == INVALID_ENTITY_ID) return;

        if (this->m_NumPendingDestroyedEntities < this->m_PendingDestroyedEntities.size()) {
            this->m_PendingDestroyedEntities[this->m_NumPendingDestroyedEntities++] = entityId;
        }
        else {
            this->m_PendingDestroyedEntities.push_back(entityId);
            this->m_NumPendingDestroyedEntities++;
        }
    }

    EntityId EntityManager::AcquireEntityId(IEntity *entity) {
        if(entity->GetId() == INVALID_ENTITY_ID) return INVALID_ENTITY_ID;

        for(auto kv : this->m_EntityHandleTable){
            if(kv.second == entity) return kv.first;
        }
        auto index = this->m_EntityHandleTable.size();
        this->m_EntityHandleTable[index] = entity;

        return index;
    }

    void EntityManager::ReleaseEntityId(EntityId id) {
        if(id == INVALID_ENTITY_ID) return;
        this->m_EntityHandleTable.erase(id);
    }

    void EntityManager::RemoveDestroyedEntities() {
        for (size_t i = 0; i < this->m_NumPendingDestroyedEntities; ++i) {
            EntityId entityId = this->m_PendingDestroyedEntities[i];
            IEntity* entity = this->m_EntityHandleTable[entityId];
            const EntityTypeId ETID = entity->GetStaticEntityTypeID();

            // get appropriate entity container and destroy entity
            auto it = this->m_EntityRegistry.find(ETID);
            if (it != this->m_EntityRegistry.end()) {
                // release entity's components
                ECS_Engine->GetComponentManager()->RemoveAllComponents(entityId);
                it->second->DestroyEntity(entity);
            }
            ReleaseEntityId(entityId);
        }

        this->m_NumPendingDestroyedEntities = 0;
    }
}