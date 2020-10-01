//
// Created by phil on 01.10.20.
//

#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include "IComponent.h"
#include "../util/FamilyTypeID.h"

namespace ECS {
    template<class C>
    class Component
    : public IComponent
    {

    public:

        static const ComponentTypeId STATIC_COMPONENT_TYPE_ID;

        Component()
        {}

        virtual ~Component()
        {}

        inline ComponentTypeId GetStaticComponentTypeID() const { return STATIC_COMPONENT_TYPE_ID; }
    };

    template<class T>
    const ComponentTypeId Component<T>::STATIC_COMPONENT_TYPE_ID = FamilyTypeID<IComponent>::Get<T>();

}


#endif //ECS_COMPONENT_H
