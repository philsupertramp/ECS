//
// Created by phil on 01.10.20.
//

#ifndef ECS_SYSTEMMANAGER_H
#define ECS_SYSTEMMANAGER_H

#include "../memory/allocator/LinearAllocator.h"
#include "ISystem.h"

namespace ECS {
    using SystemWorkStateMask	= std::vector<bool>;

    class SystemManager
    : GlobalMemoryUser
    {
        friend class ECSEngine;
    private:

        using SystemDependencyMatrix = std::vector<std::vector<bool>>;

        using SystemRegistry	= std::unordered_map<u64, ISystem*>;
        using SystemAllocator	= LinearAllocator;

        using SystemWorkOrder	= std::vector<ISystem*>;

        SystemAllocator*		m_SystemAllocator;

        SystemRegistry			m_Systems;

        SystemDependencyMatrix	m_SystemDependencyMatrix;

        SystemWorkOrder			m_SystemWorkOrder;

        // This class is not inteeded to be initialized
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(SystemManager&) = delete;

        void Update(f32 dt_ms);

    public:
        SystemManager();
        ~SystemManager();

        template<class T, class... ARGS>
        T* AddSystem(ARGS&&... systemArgs)
        {
            const u64 STID = T::STATIC_SYSTEM_TYPE_ID;


            // avoid multiple registrations of the same system
            auto it = this->m_Systems.find(STID);
            if ((this->m_Systems.find(STID) != this->m_Systems.end()) && (it->second != nullptr))
                return (T*)it->second;

            T* system = nullptr;
            void* pSystemMem = this->m_SystemAllocator->allocate(sizeof(T), alignof(T));
            if (pSystemMem != nullptr)
            {
                // create new system
                system = new (pSystemMem)T(std::forward<ARGS>(systemArgs)...);
                this->m_Systems[STID] = system;

//                Fawxy::Log::ECS->Info("System \'" + std::string(typeid(T).name()) + "\' (" + std::to_string(sizeof(T)) +" bytes) created.");
            }
            else
            {
//                Fawxy::Log::ECS->Error("Unable to create system \'" + std::string(typeid(T).name()) + "\' (" + std::to_string(sizeof(T)) +" bytes) created.");
                assert(true);
            }

            // resize dependency matrix
            if (STID + 1 > this->m_SystemDependencyMatrix.size())
            {
                this->m_SystemDependencyMatrix.resize(STID + 1);
                for (int i = 0; i < this->m_SystemDependencyMatrix.size(); ++i)
                    this->m_SystemDependencyMatrix[i].resize(STID + 1);
            }

            // add to work list
            this->m_SystemWorkOrder.push_back(system);

            return system;
        }

        template<class System_, class Dependency_>
        void AddSystemDependency(System_ target, Dependency_ dependency)
        {
            const u64 TARGET_ID = target->GetStaticSystemTypeID();
            const u64 DEPEND_ID = dependency->GetStaticSystemTypeID();

            if (this->m_SystemDependencyMatrix[TARGET_ID][DEPEND_ID] != true)
            {
                this->m_SystemDependencyMatrix[TARGET_ID][DEPEND_ID] = true;
//                Fawxy::Log::ECS->Info("added \'" + std::string(dependency->GetSystemTypeName()) + "\' as dependency tp '" + std::string(target->GetSystemTypeName()));
            }

            //this->UpdateSystemWorkOrder();
        }

        template<class Target_, class Dependency_, class... Dependencies>
        void AddSystemDependency(Target_ target, Dependency_ dependency, Dependencies&&... dependencies)
        {
            const u64 TARGET_ID = target->GetStaticSystemTypeID();
            const u64 DEPEND_ID = dependency->GetStaticSystemTypeID();

            if (this->m_SystemDependencyMatrix[TARGET_ID][DEPEND_ID] != true)
            {
                this->m_SystemDependencyMatrix[TARGET_ID][DEPEND_ID] = true;
//                Fawxy::Log::ECS->Info("added \'" + std::string(dependency->GetSystemTypeName()) + "\' as dependency tp '" + std::string(target->GetSystemTypeName()));
            }

            this->AddSystemDependency(target, std::forward<Dependencies>(dependencies)...);
        }

        void UpdateSystemWorkOrder();

        template<class T>
        inline T* GetSystem() const
        {
            auto it = this->m_Systems.find(T::STATIC_SYSTEM_TYPE_ID);

            return it != this->m_Systems.end() ? (T*)it->second : nullptr;
        }


        template<class T>
        void EnableSystem()
        {
            const SystemTypeId STID = T::STATIC_SYSTEM_TYPE_ID;

            // get system
            auto it = this->m_Systems.find(STID);
            if (it != this->m_Systems.end())
            {
                if (it->second->m_Enabled == true)
                    return;

                // enable system
                it->second->m_Enabled = true;
            }
//            else
//            {
//                Fawxy::Log::ECS->Warn("Trying to enable system [" + std::to_string(STID) + "], but system is not registered yet.");
//            }
        }


        template<class T>
        void DisableSystem()
        {
            const SystemTypeId STID = T::STATIC_SYSTEM_TYPE_ID;

            // get system
            auto it = this->m_Systems.find(STID);
            if (it != this->m_Systems.end())
            {
                if (it->second->m_Enabled == false)
                    return;

                // enable system
                it->second->m_Enabled = false;
            }
//            else
//            {
//                Fawxy::Log::ECS->Warn("Trying to disable system [" + std::to_string(STID) + "], but system is not registered yet.");
//            }
        }

        template<class T>
        void SetSystemUpdateInterval(f32 interval_ms)
        {
            const SystemTypeId STID = T::STATIC_SYSTEM_TYPE_ID;

            // get system
            auto it = this->m_Systems.find(STID);
            if (it != this->m_Systems.end())
            {
                it->second->m_UpdateInterval = interval_ms;
            }
//            else
//            {
//                Fawxy::Log::ECS->Warn("Trying to change system's [" + std::to_string(STID) + "] update interval, but system is not registered yet.");
//            }
        }

        template<class T>
        void SetSystemPriority(SystemPriority newPriority)
        {
            const SystemTypeId STID = T::STATIC_SYSTEM_TYPE_ID;

            // get system
            auto it = this->m_Systems.find(STID);
            if (it != this->m_Systems.end())
            {
                SystemPriority oldPriority = it->second->m_Priority;

                if (oldPriority == newPriority)
                    return;

                it->second->m_Priority = newPriority;

                // re-build system work order
                //this->UpdateSystemWorkOrder();
            }
//            else
//            {
//                Fawxy::Log::ECS->Warn("Trying to change system's [" + std::to_string(STID) + "] priority, but system is not registered yet.");
//            }
        }
        SystemWorkStateMask GetSystemWorkState() const;

        void SetSystemWorkState(SystemWorkStateMask mask);

        template<class... ActiveSystems>
        SystemWorkStateMask GenerateActiveSystemWorkState(ActiveSystems&&... activeSystems)
        {
            SystemWorkStateMask mask(this->m_SystemWorkOrder.size(), false);

            std::list<ISystem*> AS = { activeSystems... };
            for (auto s : AS)
            {
                for (int i = 0; i < this->m_SystemWorkOrder.size(); ++i)
                {
                    if (this->m_SystemWorkOrder[i]->GetStaticSystemTypeID() == s->GetStaticSystemTypeID())
                    {
                        mask[i] = true;
                        break;
                    }
                }
            }

            return mask;
        }
    };
}

#endif //ECS_SYSTEMMANAGER_H
