//
// Created by phil on 01.10.20.
//

#ifndef ECS_FAMILYTYPEID_CPP
#define ECS_FAMILYTYPEID_CPP

#include "../../../include/ECS/util/FamilyTypeID.h"

namespace ECS {

    class IEntity;
    class IComponent;

    template class FamilyTypeID<IEntity>;
    template class FamilyTypeID<IComponent>;
//    template class FamilyTypeID<ISystem>;
    template<> u64 FamilyTypeID<IEntity>::s_count			= 0u;
    template<> u64 FamilyTypeID<IComponent>::s_count		= 0u;
}

#endif //ECS_FAMILYTYPEID_CPP
