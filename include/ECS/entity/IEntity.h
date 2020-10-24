//
// Created by phil on 01.10.20.
//

#ifndef ECS_IENTITY_H
#define ECS_IENTITY_H

#include "../Platform.h"
#include "../Engine.h"
#include "../component/ComponentManager.h"

namespace ECS {
    class IEntity
    {
        EntityId m_EntityID;
        bool m_IsActive;
    public:
        IEntity();
        ~IEntity() = default;

        template<class T>
        inline T* GetComponent(){
            if (this) return ECS_Engine->GetComponentManager()->GetComponent<T>(this->m_EntityID);
            return nullptr;
        }

        template<class T, class ...A>
        inline T* CreateComponent(A&&... param){
            if(this) return ECS_Engine->GetComponentManager()->AddComponent<T>(this->m_EntityID, std::forward<A>(param)...);
            return nullptr;
        }

        template<class T>
        inline void RemoveComponent(){
            if(this) ECS_Engine->GetComponentManager()->RemoveComponent<T>(this->m_EntityID);
        }

        inline bool operator==(const IEntity& rhs) const { return this->m_EntityID == rhs.m_EntityID; }
        inline bool operator!=(const IEntity& rhs) const { return this->m_EntityID != rhs.m_EntityID; }
        inline bool operator==(const IEntity* rhs) const { return this->m_EntityID == rhs->m_EntityID; }
        inline bool operator!=(const IEntity* rhs) const { return this->m_EntityID != rhs->m_EntityID; }


        inline const EntityId GetId() const { return this ? m_EntityID : INVALID_ENTITY_ID; }
        virtual const EntityTypeId GetStaticEntityTypeID() const = 0;
        inline void SetActive(bool val) { m_IsActive = val; }
        inline bool IsActive() { return m_IsActive; }

        virtual void Render() {}
    };
}

#endif //ECS_IENTITY_H
