//
// Created by phil on 01.10.20.
//

#ifndef ECS_ENGINE_H
#define ECS_ENGINE_H

#include "Platform.h"
#include "event/EventHandler.h"
#include "event/EventDelegate.h"

namespace ECS {
    class EntityManager;
    class ComponentManager;
    class SystemManager;
    namespace Event {
        class IEvent;
        class IEventListener;
        class EventHandler;
    }
    class ECSEngine
    {

        friend class IEntity;
        friend class IComponent;
        friend class ISystem;

        friend class Event::IEvent;

        friend class Event::IEventListener;
        friend class Event::EventHandler;

        friend class EntityManager;

        EntityManager*				ECS_EntityManager;
        ComponentManager*			ECS_ComponentManager;

        SystemManager*				ECS_SystemManager;

        Event::EventHandler*		ECS_EventHandler;
        ECSEngine(const ECSEngine&) = delete;
        ECSEngine& operator=(ECSEngine&) = delete;

        // Add event callback
        template<class E>
        inline void SubscribeEvent(Event::IEventDelegate* const eventDelegate)
        {
            ECS_EventHandler->AddEventCallback<E>(eventDelegate);
        }

        // Remove event callback
        void UnsubscribeEvent(Event::IEventDelegate* eventDelegate);

    public:
        float				ECS_EngineTime;

        ECSEngine();

        ~ECSEngine();


        inline EntityManager* GetEntityManager() { return ECS_EntityManager; }

        inline ComponentManager* GetComponentManager() { return ECS_ComponentManager; }

        inline SystemManager* GetSystemManager() { return ECS_SystemManager; }

        void Update(f32 tick_ms);

        template<class E, class... ARGS>
        void SendEvent(ARGS&&... eventArgs)
        {
            ECS_EventHandler->Send<E>(std::forward<ARGS>(eventArgs)...);
        }

    };
}

#endif //ECS_ENGINE_H
