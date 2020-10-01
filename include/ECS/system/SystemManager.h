//
// Created by phil on 01.10.20.
//

#ifndef ECS_SYSTEMMANAGER_H
#define ECS_SYSTEMMANAGER_H

namespace ECS {
    using SystemWorkStateMask	= std::vector<bool>;

    class SystemManager
    : GlobalMemoryUser
    {
    private:

        using SystemDependencyMatrix = std::vector<std::vector<bool>>;

        using SystemRegistry	= std::unordered_map<u64, ISystem*>;
        using SystemAllocator	= Memory::Allocator::LinearAllocator;

        using SystemWorkOrder	= std::vector<ISystem*>;

        SystemAllocator*		m_SystemAllocator;

        SystemRegistry			m_Systems;

        SystemDependencyMatrix	m_SystemDependencyMatrix;

        SystemWorkOrder			m_SystemWorkOrder;

        // This class is not inteeded to be initialized
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(SystemManager&) = delete;



    };
}

#endif //ECS_SYSTEMMANAGER_H
