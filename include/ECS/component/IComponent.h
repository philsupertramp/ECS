//
// Created by phil on 01.10.20.
//

#ifndef ECS_ICOMPONENT_H
#define ECS_ICOMPONENT_H

#include "../Platform.h"


namespace ECS {
    class IComponent
    {
    protected:
        friend class ComponentManager;
        ComponentId m_HashValue;
        ComponentId		m_ComponentID;
        EntityId		m_Owner;
        bool			m_Enabled;
    public:
        IComponent();
        ~IComponent();

        inline const bool operator==(const IComponent& other) const { return m_HashValue == other.m_HashValue; }
        inline const bool operator!=(const IComponent& other) const { return m_HashValue == other.m_HashValue; }

        inline const ComponentId GetComponentId() const { return this->m_ComponentID; }
        inline const EntityId GetOwner() const { return this->m_Owner; }
        inline void SetActive(bool state) { this->m_Enabled = state; }
        inline bool IsActive() const { return this->m_Enabled; }

    };

}
#endif //ECS_ICOMPONENT_H
