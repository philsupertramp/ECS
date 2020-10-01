//
// Created by phil on 01.10.20.
//

#ifndef ECS_ENTITYMANAGER_H
#define ECS_ENTITYMANAGER_H

#include "../Platform.h"
#include "../API.h"
#include "../memory/MemoryChunkAllocator.h"
#include "IEntity.h"

namespace ECS {
    using EntityHandleTable = std::map<EntityId,IEntity*>;

    class EntityManager: GlobalMemoryUser
    {

        class IEntityContainer
        {
        public:

            virtual ~IEntityContainer()
            {}

            virtual const char* GetEntityContainerTypeName() const = 0;

            virtual void DestroyEntity(IEntity* object) = 0;

        }; // class IEntityContainer

        // EntityContainer
    public:
        template<class T>
        class EntityContainer :public MemoryChunkAllocator<T, ENITY_T_CHUNK_SIZE>, public IEntityContainer
        {

            EntityContainer(const EntityContainer&) = delete;
            EntityContainer& operator=(EntityContainer&) = delete;

        public:

            EntityContainer():MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>("EntityManager")
            {}

            virtual ~EntityContainer()
            {}

            virtual const char* GetEntityContainerTypeName() const override {
                static const char* ENTITY_TYPE_NAME{ typeid(T).name() };
                return ENTITY_TYPE_NAME;
            }

            /* TODO: might need check */
            void DestroyEntity(IEntity* object) {
                // call d'tor
                object->~IEntity();

                this->DestroyObject(object);
            }

        };

    private:
        using EntityRegistry = std::unordered_map<EntityTypeId, IEntityContainer*>;
        EntityRegistry m_EntityRegistry;

        using PendingDestroyedEntities = std::vector<EntityId>;
        PendingDestroyedEntities m_PendingDestroyedEntities;
        size_t m_NumPendingDestroyedEntities;

    private:

        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(EntityManager&) = delete;

        EntityHandleTable m_EntityHandleTable;

    public:
        template<class T>
        inline EntityContainer<T>* GetEntityContainer() {
            EntityTypeId EID = T::STATIC_ENTITY_TYPE_ID;

            auto it = this->m_EntityRegistry.find(EID);
            EntityContainer<T>* ec = nullptr;

            if (it == this->m_EntityRegistry.end()) {
                ec = new EntityContainer<T>();
                this->m_EntityRegistry[EID] = ec;
            }
            else {
                ec = (EntityContainer<T> *) it->second;
            }
            assert(ec != nullptr && "Failed to create EntityContainer<T>!");
            return ec;
        }
        EntityId AcquireEntityId(IEntity* entity);
        void ReleaseEntityId(EntityId id);

    public:

        EntityManager();
        ~EntityManager();

        template<class T, class... ARGS>
        IEntity* CreateEntity(ARGS&&... args) {
            // aqcuire memory for new entity object of type T
            void* pObjectMemory = GetEntityContainer<T>()->CreateObject();

            // create entity inplace
            IEntity* entity = new (pObjectMemory)T(std::forward<ARGS>(args)...);

            return entity;
        }

        void DestroyEntity(EntityId entityId);

        inline IEntity* GetEntity(EntityId entityId) {
            return entityId == INVALID_ENTITY_ID ? nullptr : this->m_EntityHandleTable[entityId];
        }

        void RemoveDestroyedEntities();
    };
}

#endif //ECS_ENTITYMANAGER_H
