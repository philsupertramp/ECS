//
// Created by phil on 01.10.20.
//

#include "../../include/ECS/component/ComponentManager.h"
#include "../../include/ECS/util/FamilyTypeID.h"


namespace ECS {

    ComponentManager::ComponentManager() {
        const size_t NUM_COMPONENTS { FamilyTypeID<IComponent>::Get() };

        this->m_EntityComponentMap.resize(ENITY_LUT_GROW);
        for (auto i = 0; i < ENITY_LUT_GROW; ++i)
            this->m_EntityComponentMap[i].resize(NUM_COMPONENTS, INVALID_COMPONENT_ID);
    }

    ComponentManager::~ComponentManager(){
        for (auto cc : this->m_ComponentContainerRegistry){
            delete cc.second;
            cc.second = nullptr;
        }
    }

    void ComponentManager::UnmapEntityComponent(EntityId entityId, ComponentId componentId,
                                                ComponentTypeId componentTypeId) {

        assert(this->m_EntityComponentMap[entityId][componentTypeId] == componentId && "FATAL: Entity Component ID mapping corruption!");

        // free mapping
        this->m_EntityComponentMap[entityId][componentTypeId] = INVALID_COMPONENT_ID;
    }

    void ComponentManager::MapEntityComponent(EntityId entityId, ComponentId componentId,
                                              ComponentTypeId componentTypeId) {

        static const size_t NUM_COMPONENTS { FamilyTypeID<IComponent>::Get() };

        if ((this->m_EntityComponentMap.size() - 1) < (int)entityId)
        {
            size_t oldSize = this->m_EntityComponentMap.size();

            // we scale this map size along the entity lookup table size
            size_t newSize = oldSize + ENITY_LUT_GROW;

            this->m_EntityComponentMap.resize(newSize);

            for (auto i = oldSize; i < newSize; ++i)
                this->m_EntityComponentMap[i].resize(NUM_COMPONENTS, INVALID_COMPONENT_ID);
        }
        if(this->m_EntityComponentMap[entityId].size() == 0){
            auto saveGame = this->m_EntityComponentMap[entityId];
            this->m_EntityComponentMap[entityId].resize(componentId + 1);
            int i = 0;
            for(auto elem : saveGame){
                this->m_EntityComponentMap[entityId][i] = elem;
                i++;
            }
        }

        // create mapping
        this->m_EntityComponentMap[entityId][componentTypeId] = componentId;
    }

    void ComponentManager::ReleaseComponentId(ComponentId id) {
        assert((id != INVALID_COMPONENT_ID && id < this->m_ComponentLUT.size()) && "Invalid component id");
        this->m_ComponentLUT[id] = nullptr;
    }

    ComponentId ComponentManager::AcquireComponentId(IComponent *component) {
        int i = 0;
        for (; i < this->m_ComponentLUT.size(); ++i)
        {
            if (this->m_ComponentLUT[i] == nullptr)
            {
                this->m_ComponentLUT[i] = component;
                return i;
            }
        }

        // increase component LUT size
        this->m_ComponentLUT.resize(this->m_ComponentLUT.size() + COMPONENT_LUT_GROW, nullptr);

        this->m_ComponentLUT[i] = component;
        return i;
    }
}