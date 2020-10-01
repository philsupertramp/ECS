//
// Created by phil on 01.10.20.
//

#ifndef ECS_ENGINE_H
#define ECS_ENGINE_H

#include "Platform.h"

namespace ECS {
    class EntityManager;
    class ComponentManager;
    class SystemManager;
    class ECSEngine
    {

        float				ECS_EngineTime;
        EntityManager*				ECS_EntityManager;
        ComponentManager*			ECS_ComponentManager;

        SystemManager*				ECS_SystemManager;

//        Event::EventHandler*		ECS_EventHandler;
        ECSEngine(const ECSEngine&) = delete;
        ECSEngine& operator=(ECSEngine&) = delete;

    public:

        ECSEngine();

        ~ECSEngine();


        inline EntityManager* GetEntityManager() { return ECS_EntityManager; }

        inline ComponentManager* GetComponentManager() { return ECS_ComponentManager; }

        inline SystemManager* GetSystemManager() { return ECS_SystemManager; }

        void Update(f32 tick_ms);
    };
}

#endif //ECS_ENGINE_H
