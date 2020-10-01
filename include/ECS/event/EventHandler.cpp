//
// Created by phil on 01.10.20.
//

#include "EventHandler.h"

namespace ECS::Event{
    EventHandler::EventHandler() {

        // allocate memory from global memory manager
        this->m_EventMemoryAllocator = new EventMemoryAllocator(ECS_EVENT_MEMORY_BUFFER_SIZE, Allocate(ECS_EVENT_MEMORY_BUFFER_SIZE, "EventHandler"));

        this->m_EventStorage.reserve(1024);
    }

    EventHandler::~EventHandler() {
        for (auto it = this->m_EventDispatcherMap.begin(); it != this->m_EventDispatcherMap.end(); ++it) {
            delete (*it).second;
            (*it).second = nullptr;
        }

        this->m_EventDispatcherMap.clear();

        // Release allocated memory
        Free((void*)this->m_EventMemoryAllocator->GetMemoryAddress0());

        delete this->m_EventMemoryAllocator;
        this->m_EventMemoryAllocator = nullptr;

    }
}