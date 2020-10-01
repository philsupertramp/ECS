//
// Created by phil on 01.10.20.
//

#ifndef ECS_IEVENT_H
#define ECS_IEVENT_H

#include "../Platform.h"

namespace ECS::Event {

    class IEvent {
        friend class ECSEngine;
    private:
        EventTypeId m_TypeId;
        EventTimestamp m_TimeCreated;
    public:
        IEvent(EventTypeId typeId);

        inline const EventTypeId GetEventTypeID() const { return this->m_TypeId; }
        inline const EventTimestamp GetTimeCreated() const { return this->m_TimeCreated; }

    };

}

#endif //ECS_IEVENT_H
