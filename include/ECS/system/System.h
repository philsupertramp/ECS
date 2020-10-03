//
// Created by phil on 01.10.20.
//

#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include "ISystem.h"
#include "../Engine.h"
#include "SystemManager.h"

namespace ECS {
    template <class S>
    class System
    : public ISystem
    {
    public:

        static const SystemTypeId STATIC_SYSTEM_TYPE_ID;

        System()
        { }

        virtual ~System()
        { }

        void SetActive(bool val) { m_Enabled = val; }
        bool IsActive() { return m_Enabled; }

        virtual inline const SystemTypeId GetStaticSystemTypeID() const {
            return STATIC_SYSTEM_TYPE_ID;
        }

        virtual inline const char* GetSystemTypeName() const override {
            static const char* SYSTEM_TYPE_NAME { typeid(S).name() };
            return SYSTEM_TYPE_NAME;
        }

        template<class... Dependencies>
        void AddDependencies(Dependencies&&... dependencies) {
            ECS_Engine->GetSystemManager()->AddSystemDependency(this, std::forward<Dependencies>(dependencies)...);
        }


        virtual void PreUpdate(f32 dt) override
        {}

        virtual void Update(f32 dt) override
        {}

        virtual void PostUpdate(f32 dt) override
        {}

    };

    template<class T>
    const SystemTypeId System<T>::STATIC_SYSTEM_TYPE_ID = FamilyTypeID<ISystem>::Get<T>();
}

#endif //ECS_SYSTEM_H
