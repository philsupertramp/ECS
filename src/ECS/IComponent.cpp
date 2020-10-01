//
// Created by phil on 01.10.20.
//

#include "../../include/ECS/component/IComponent.h"
#include "../../include/ECS/Engine.h"
#include "../../include/ECS/API.h"
#include "../../include/ECS/component/ComponentManager.h"

namespace ECS {

    IComponent::IComponent()
    : m_Owner(INVALID_ENTITY_ID),
    m_Enabled(true)
    {
        // aqcuire new unique component id
        this->m_ComponentID = ECS_Engine->GetComponentManager()->AcquireComponentId(this);
    }

    IComponent::~IComponent() {
        // release id so it can be reused
        ECS_Engine->GetComponentManager()->ReleaseComponentId(this->m_ComponentID);
    }
}

