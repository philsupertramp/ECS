//
// Created by phil on 01.10.20.
//

#include "../../include/ECS/Engine.h"
#include "../../include/ECS/component/ComponentManager.h"
#include "../../include/ECS/entity/EntityManager.h"
#include "../../include/ECS/system/SystemManager.h"

namespace ECS {
    ECSEngine::ECSEngine(){
        ECS_EngineTime			= 0;
        ECS_ComponentManager	= new ComponentManager();
        ECS_EntityManager		= new EntityManager();
        ECS_SystemManager       = new SystemManager();
        ECS_EventHandler       = new Event::EventHandler();
    }

    ECSEngine::~ECSEngine() {
        delete ECS_EntityManager;
        ECS_EntityManager = nullptr;

        delete ECS_ComponentManager;
        ECS_ComponentManager = nullptr;

        delete ECS_SystemManager;
        ECS_SystemManager = nullptr;

        delete ECS_EventHandler;
        ECS_EventHandler = nullptr;
    }

    void ECSEngine::Update(f32 tick_ms) {
        // Advance engine time
        ECS_EngineTime += tick_ms;

        // Update all running systems
        ECS_SystemManager->Update(tick_ms);
        ECS_EventHandler->DispatchEvents();

        // Finalize pending destroyed entities
        ECS_EntityManager->RemoveDestroyedEntities();
        ECS_EventHandler->DispatchEvents();
    }

    void ECSEngine::UnsubscribeEvent(Event::IEventDelegate *eventDelegate) {
        ECS_EventHandler->RemoveEventCallback(eventDelegate);
    }
}