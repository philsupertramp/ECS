//
// Created by phil on 01.10.20.
//

#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "IEntity.h"
#include "../util/FamilyTypeID.h"

namespace ECS {
    template<class E>
    class Entity
    : public IEntity
    {
    public:
        Entity(){}
        ~Entity() = default;

        static const EntityTypeId STATIC_ENTITY_TYPE_ID;
        virtual const EntityTypeId GetStaticEntityTypeID() const override { return STATIC_ENTITY_TYPE_ID; }
    };

    // set unique type id for this Entity<T>
    template<class E>
    const EntityTypeId Entity<E>::STATIC_ENTITY_TYPE_ID = FamilyTypeID<IEntity>::Get<E>();
}

#endif //ECS_ENTITY_H
