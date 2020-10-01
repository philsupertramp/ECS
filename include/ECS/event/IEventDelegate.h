//
// Created by phil on 01.10.20.
//

#ifndef ECS_IEVENTDELEGATE_H
#define ECS_IEVENTDELEGATE_H

#include "../Platform.h"
#include "IEvent.h"

namespace ECS::Event {
    class IEventDelegate
    {
    public:

        virtual inline void invoke(const IEvent* const e) = 0;

        virtual inline EventDelegateId GetDelegateId() const = 0;

        virtual inline u64 GetStaticEventTypeId() const = 0;

        virtual bool operator==(const IEventDelegate* other) const = 0;

        virtual IEventDelegate* clone() = 0;

    }; // class IEventDelegate

}

#endif //ECS_IEVENTDELEGATE_H
