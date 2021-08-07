//
// Created by phil on 01.10.20.
//

#ifndef ECS_COMPONENTMANAGER_H
#define ECS_COMPONENTMANAGER_H

#include "../API.h"
#include "../memory/MemoryChunkAllocator.h"
#include "IComponent.h"

namespace ECS {

    class ComponentManager: GlobalMemoryUser
    {

        friend class IComponent;

        class IComponentContainer
        {
        public:
            virtual ~IComponentContainer() {}
            virtual const char* GetComponentContainerTypeName() const = 0;
            virtual void DestroyComponent(IComponent* object) = 0;
        };

        template<class T>
        class ComponentContainer : public MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>, public IComponentContainer
        {
            ComponentContainer(const ComponentContainer&) = delete;
            ComponentContainer& operator=(ComponentContainer&) = delete;

        public:

            ComponentContainer() : MemoryChunkAllocator<T, COMPONENT_T_CHUNK_SIZE>("ComponentManager")
            {}

            virtual ~ComponentContainer()
            {}

            virtual const char* GetComponentContainerTypeName() const override
            {
                static const char* COMPONENT_TYPE_NAME{ typeid(T).name() };
                return COMPONENT_TYPE_NAME;
            }

            virtual void DestroyComponent(IComponent* object) override{
                // call d'tor
                object->~IComponent();
                this->DestroyObject(object);
            }
        };

        ComponentManager(const ComponentManager&) = delete;
        ComponentManager& operator=(ComponentManager&) = delete;

        using ComponentContainerRegistry = std::unordered_map<ComponentTypeId, IComponentContainer*>;

        ComponentContainerRegistry m_ComponentContainerRegistry;

        template<class T>
        inline ComponentContainer<T>* GetComponentContainer()
        {
            ComponentTypeId CID = T::STATIC_COMPONENT_TYPE_ID;

            auto it = this->m_ComponentContainerRegistry.find(CID);
            ComponentContainer<T>* cc = nullptr;

            if (it == this->m_ComponentContainerRegistry.end())
            {
                cc = new ComponentContainer<T>();
                this->m_ComponentContainerRegistry[CID] = cc;
            }
            else
                cc = static_cast<ComponentContainer<T>*>(it->second);

            assert(cc != nullptr && "Failed to create ComponentContainer<T>!");
            return cc;
        }

        using ComponentLookupTable = std::vector<IComponent*>;
        ComponentLookupTable	m_ComponentLUT;

        using EntityComponentMap = std::vector<std::vector<ComponentId>>;
        EntityComponentMap		m_EntityComponentMap;


        ComponentId	AcquireComponentId(IComponent* component);
        void		ReleaseComponentId(ComponentId id);

        void		MapEntityComponent(EntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId);
        void		UnmapEntityComponent(EntityId entityId, ComponentId componentId, ComponentTypeId componentTypeId);

    public:

        template<class T>
        using TComponentIterator = typename ComponentContainer<T>::iterator;

        ComponentManager();
        ~ComponentManager();


        template<class T, class ...ARGS>
        T* AddComponent(const EntityId entityId, ARGS&&... args)
        {
            // hash operator for hashing entity and component ids
            static constexpr std::hash<ComponentId> ENTITY_COMPONENT_ID_HASHER { std::hash<ComponentId>() };

            const ComponentTypeId CTID	= T::STATIC_COMPONENT_TYPE_ID;

            // aqcuire memory for new component object of type T
            void* pObjectMemory			= GetComponentContainer<T>()->CreateObject();

            // create component inplace
            IComponent* component		= new (pObjectMemory)T(std::forward<ARGS>(args)...);

            ComponentId componentId		= component->m_ComponentID;

            component->m_Owner			= entityId;
            component->m_HashValue		= ENTITY_COMPONENT_ID_HASHER(entityId) ^ (ENTITY_COMPONENT_ID_HASHER(componentId) << 1);

            // create mapping from entity id its component id
            MapEntityComponent(entityId, componentId, CTID);

            return static_cast<T*>(component);
        }

        template<class T>
        void RemoveComponent(const EntityId entityId)
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            const ComponentId componentId = this->m_EntityComponentMap[entityId][CTID];

            IComponent* component = this->m_ComponentLUT[componentId];

            assert(component != nullptr && "FATAL: Trying to remove a component which is not used by this entity!");

            // release object memory
            GetComponentContainer<T>()->DestroyObject(component);

            // unmap entity id to component id
            UnmapEntityComponent(entityId, componentId, CTID);
        }

        void RemoveAllComponents(const EntityId entityId)
        {
            static const size_t NUM_COMPONENTS = this->m_EntityComponentMap[entityId].size();

            for (ComponentTypeId componentTypeId = 0; componentTypeId < NUM_COMPONENTS; ++componentTypeId)
            {
                const ComponentId componentId = this->m_EntityComponentMap[entityId][componentTypeId];
                if (componentId == INVALID_COMPONENT_ID)
                    continue;

                IComponent* component = this->m_ComponentLUT[componentId];
                if (component != nullptr)
                {
                    // get appropriate component container
                    auto it = this->m_ComponentContainerRegistry.find(componentTypeId);
                    if (it != this->m_ComponentContainerRegistry.end())
                    {
                        it->second->DestroyComponent(component);
                        //                    else
                        //                        assert(false && "Trying to release a component that wasn't created by ComponentManager!");

                        // unmap entity id to component id
                        UnmapEntityComponent(entityId, componentId, componentTypeId);
                    }
                }
            }
        }

        template<class T>
        T* GetComponent(const EntityId entityId)
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;


            if(this->m_EntityComponentMap[entityId].empty())
                return nullptr;

            const ComponentId componentId = this->m_EntityComponentMap[entityId][CTID];

            // entity has no component of type T
            if (componentId == INVALID_COMPONENT_ID)
                return nullptr;

            return static_cast<T*>(this->m_ComponentLUT[componentId]);
        }

        template<class T>
        inline TComponentIterator<T> begin()
        {
            return GetComponentContainer<T>()->begin();
        }
        template<class T>
        inline TComponentIterator<T> end()
        {
            return GetComponentContainer<T>()->end();
        }
    };


}

#endif //ECS_COMPONENTMANAGER_H
