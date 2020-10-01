//
// Created by phil on 01.10.20.
//

#include "../include/ECS/API.h"
#include "../include/ECS/Engine.h"

namespace ECS {
    MemoryManager* ECSMemoryManager = new MemoryManager();

    GlobalMemoryUser::GlobalMemoryUser(): ECS_MEMORY_MANAGER(ECSMemoryManager) {

    }

    GlobalMemoryUser::~GlobalMemoryUser() {

    }

    const void *GlobalMemoryUser::Allocate(size_t memSize, const char *user) {
        return ECS_MEMORY_MANAGER->Allocate(memSize, user);
    }

    void GlobalMemoryUser::Free(void *pMem) {
        ECS_MEMORY_MANAGER->Free(pMem);
    }
    ECSEngine* ECS_Engine = new ECSEngine();

}