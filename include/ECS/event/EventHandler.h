//
// Created by phil on 01.10.20.
//

#ifndef ECS_EVENTHANDLER_H
#define ECS_EVENTHANDLER_H

#include "../API.h"
#include "IEvent.h"
#include "../memory/allocator/LinearAllocator.h"
#include "EventDispatcher.h"

namespace ECS::Event {
    class EventHandler: GlobalMemoryUser
    {
// allow IEventListener access private methods for Add/Remove callbacks
        friend class ECSEngine;


        using EventDispatcherMap = std::unordered_map<EventTypeId, IEventDispatcher*>;

        using EventStorage = std::vector<IEvent*>;

        using EventMemoryAllocator = LinearAllocator;

    private:

        EventHandler(const EventHandler&);
        EventHandler& operator=(EventHandler&);

        EventDispatcherMap			m_EventDispatcherMap;


        EventMemoryAllocator*		m_EventMemoryAllocator;

        // Holds a list of all sent events since last EventHandler::DispatchEvents() call
        EventStorage				m_EventStorage;

    public:
        // Add event callback
        template<class E>
        inline void AddEventCallback(IEventDelegate* const eventDelegate)
        {
            EventTypeId ETID = E::STATIC_EVENT_TYPE_ID;

            EventDispatcherMap::const_iterator iter = this->m_EventDispatcherMap.find(ETID);
            if (iter == this->m_EventDispatcherMap.end())
            {
                std::pair<EventTypeId, IEventDispatcher*> kvp(ETID, new EventDispatcher<E>());

                kvp.second->AddEventCallback(eventDelegate);

                this->m_EventDispatcherMap.insert(kvp);
            }
            else
            {
                this->m_EventDispatcherMap[ETID]->AddEventCallback(eventDelegate);
            }

        }

        // Remove event callback
        inline void RemoveEventCallback(IEventDelegate* eventDelegate)
        {
            auto typeId = eventDelegate->GetStaticEventTypeId();
            EventDispatcherMap::const_iterator iter = this->m_EventDispatcherMap.find(typeId);
            if (iter != this->m_EventDispatcherMap.end())
            {
                this->m_EventDispatcherMap[typeId]->RemoveEventCallback(eventDelegate);
            }
        }



    public:

        EventHandler();
        ~EventHandler();

        // clear buffer, that is, simply reset index buffer
        inline void ClearEventBuffer()
        {
            this->m_EventMemoryAllocator->clear();
            this->m_EventStorage.clear();
        }

        inline void ClearEventDispatcher()
        {
            this->m_EventDispatcherMap.clear();
        }

        template<class E, class... ARGS>
        void Send(ARGS&&... eventArgs)
        {
            // check if type of object is trivially copyable
            static_assert(std::is_trivially_copyable<E>::value, "Event is not trivially copyable.");


            // allocate memory to store event data
            void* pMem = this->m_EventMemoryAllocator->allocate(sizeof(E), alignof(E));

            // add new event to buffer and event storage
            if (pMem != nullptr)
            {
                this->m_EventStorage.push_back(new (pMem)E(std::forward<ARGS>(eventArgs)...));
            }
//            else
//            {
//                Fawxy::Log::ECS->Warn("Event buffer is full! Cut off new incoming events !!!");
//            }
        }

        // dispatches all stores events and clears buffer
        void DispatchEvents()
        {
            size_t lastIndex = this->m_EventStorage.size();
            size_t thisIndex = 0;

            while (thisIndex < lastIndex)
            {
                auto event = this->m_EventStorage[thisIndex++];
                if (event == nullptr)
                {
//                    Fawxy::Log::ECS->Error("Skip corrupted event. " + std::to_string(event->GetEventTypeID()));
                    continue;
                }

                auto it = this->m_EventDispatcherMap.find(event->GetEventTypeID());
                if (it == this->m_EventDispatcherMap.end())
                    continue;

                it->second->Dispatch(event);

                // update last index, after dispatch operation there could be new events
                lastIndex = this->m_EventStorage.size();
            }

            // reset
            ClearEventBuffer();
        }
    };
}


#endif //ECS_EVENTHANDLER_H
