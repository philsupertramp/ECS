//
// Created by phil on 01.10.20.
//

#ifndef ECS_EVENT_H
#define ECS_EVENT_H

#include "IEvent.h"

namespace ECS::Event {

    template <class E>
    class Event
    : public IEvent
    {
    public:
        static const EventTypeId STATIC_EVENT_TYPE_ID;

        Event() : IEvent(STATIC_EVENT_TYPE_ID){}
    };

    template<class T>
    const EventTypeId Event<T>::STATIC_EVENT_TYPE_ID { typeid(T).hash_code() };

}

#endif //ECS_EVENT_H
