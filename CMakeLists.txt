cmake_minimum_required(VERSION 3.18)
project(ECS)

# In case you want to build the example, uncomment this line
#set(ECS_BUILD_EXAMPLE)

set(CMAKE_CXX_STANDARD 20)

set(ECS_SOURCES
    src/ECS/IEntity.cpp
    src/ECS/IComponent.cpp
    src/ECS/util/FamilyTypeID.cpp
    src/ECS/EntityManager.cpp
    src/ECS/Engine.cpp

    src/ECS/memory/allocator/IAllocator.cpp
    src/ECS/memory/allocator/LinearAllocator.cpp
    src/ECS/memory/allocator/PoolAllocator.cpp
    src/ECS/memory/allocator/StackAllocator.cpp
    src/ECS/memory/ECSMM.cpp
    src/API.cpp
    src/ECS/ComponentManager.cpp
    src/ECS/ISystem.cpp
    src/ECS/SystemManager.cpp
)

set(ECS_HEADERS
        include/ECS/entity/IEntity.h
        include/ECS/Platform.h
        include/ECS/component/IComponent.h
        include/ECS/entity/Entity.h
        include/ECS/util/FamilyTypeID.h
        include/ECS/component/Component.h
        include/ECS/ECS.h
        include/ECS/Engine.h
        include/ECS/entity/EntityManager.h
        include/ECS/component/ComponentManager.h
    
        include/ECS/memory/allocator/IAllocator.h
        include/ECS/memory/allocator/LinearAllocator.h
        include/ECS/memory/allocator/PoolAllocator.h
        include/ECS/memory/allocator/StackAllocator.h
        include/ECS/memory/ECSMM.h
        include/ECS/memory/MemoryChunkAllocator.h

        include/ECS/API.h
        include/ECS/system/ISystem.h
        include/ECS/system/System.h
        include/ECS/system/SystemManager.h
)

add_library(ECS SHARED ${ECS_SOURCES} ${ECS_HEADERS})

set_property(TARGET ECS PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

if (ECS_BUILD_EXAMPLE)
    add_executable(ECS_Test main.cpp example.h)
    target_include_directories(ECS_Test PUBLIC include)
    target_link_libraries(ECS_Test ECS)
endif (ECS_BUILD_EXAMPLE)