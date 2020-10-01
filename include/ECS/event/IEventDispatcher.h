//
// Created by phil on 01.10.20.
//

#ifndef ECS_IEVENTDISPATCHER_H
#define ECS_IEVENTDISPATCHER_H

#include "IEvent.h"
#include "EventDelegate.h"

namespace ECS::Event {
    class IEventDispatcher
    {
    public:

        virtual ~IEventDispatcher()
        {}

        virtual void Dispatch(IEvent* event) = 0;

        virtual void AddEventCallback(IEventDelegate* const eventDelegate) = 0;

        virtual void RemoveEventCallback(IEventDelegate* eventDelegate) = 0;

        virtual inline size_t GetEventCallbackCount() const = 0;
    };
}

#endif //ECS_IEVENTDISPATCHER_H
