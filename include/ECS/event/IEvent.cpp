//
// Created by phil on 01.10.20.
//

#include "IEvent.h"
#include "../API.h"
#include "../Engine.h"

namespace ECS::Event{
    IEvent::IEvent(EventTypeId typeId) :
            m_TypeId(typeId)
    {
        assert(ECS::ECS_Engine != nullptr && "ECS engine not initialized!");
        this->m_TimeCreated = ECS::ECS_Engine->ECS_EngineTime;
    }
}
