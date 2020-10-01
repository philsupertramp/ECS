//
// Created by phil on 01.10.20.
//

#include "../../include/ECS/entity/IEntity.h"
#include "../../include/ECS/entity/EntityManager.h"

namespace ECS {

    IEntity::IEntity():
        m_IsActive(true)
    {
        this->m_EntityID = ECS_Engine->GetEntityManager()->AcquireEntityId(this);
    }
}


