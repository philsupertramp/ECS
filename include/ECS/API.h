//
// Created by phil on 01.10.20.
//

#ifndef ECS_API_H
#define ECS_API_H

#include "memory/ECSMM.h"

#define ENITY_LUT_GROW						1024

#define ENITY_T_CHUNK_SIZE					512

#define COMPONENT_LUT_GROW					1024

#define COMPONENT_T_CHUNK_SIZE				512

// 4MB
#define ECS_EVENT_MEMORY_BUFFER_SIZE		4194304

// 8MB
#define ECS_SYSTEM_MEMORY_BUFFER_SIZE		8388608

namespace ECS {

    class  MemoryManager;
    extern MemoryManager*				ECSMemoryManager;
    class GlobalMemoryUser
    {
    private:

        MemoryManager* ECS_MEMORY_MANAGER;

    public:

        GlobalMemoryUser();
        virtual ~GlobalMemoryUser();

        const void* Allocate(size_t memSize, const char* user = nullptr);
        void Free(void* pMem);
    };

    class ECSEngine;
    extern ECSEngine* ECS_Engine;

}

#endif //ECS_API_H
