//
// Created by phil on 01.10.20.
//

#include "IEventListener.h"
#include "../API.h"
#include "../Engine.h"

namespace ECS::Event {
    void IEventListener::UnregisterAllEventCallbacks() {

        // unsubscribe from all subscribed events
        for (auto cb : this->m_RegisteredCallbacks) {
            ECS_Engine->UnsubscribeEvent(cb);
        }

        this->m_RegisteredCallbacks.clear();
    }

    IEventListener::~IEventListener() {

        // unsubscribe from all subscribed events
        UnregisterAllEventCallbacks();
    }

    IEventListener::IEventListener() { }
}