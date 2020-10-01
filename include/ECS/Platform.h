///-------------------------------------------------------------------------------------------------
/// File:	include\Platform.h.
///
/// Summary:	Declares the platform specifics.

#pragma once

#ifndef __PLATFORM_H__
#define __PLATFORM_H__



// Check if using 64-Bit architecture
#if (defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_X64))
#define ECS_64BIT 1

// Check if using 32-Bit architecture
#elif (defined(_WIN32) && !defined(_WIN64)) || defined(_M_IX86)
#define ECS_32BIT 1
#endif



// Platform includes
#include <typeinfo>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>

#include <assert.h>
#include <memory>

#include <string>
#include <math.h>

#include <chrono>
#include <utility>


namespace ECS
{

    // signed integer type
    using i8			= int8_t;
    using i16			= int16_t;
    using i32			= int32_t;

#ifdef ECS_64BIT
    using i64			= int64_t;
#else
    using i64			= int32_t;
#endif

    // unsigned integer type
    using u8			= uint8_t;
    using u16			= uint16_t;
    using u32			= uint32_t;
#ifdef ECS_64BIT
    using u64			= uint64_t;
#else
    using u64			= uint32_t;
#endif

    // floating point
    using f32			= float_t;
    using f64			= double_t;

    // pointer
    using iptr			= intptr_t;
    using uptr			= uintptr_t;

    using ObjectID		    = size_t;
    using TypeID		    = size_t;
    using EntityId          = size_t;
    using EntityTypeId		= TypeID;
    using ComponentId		= ObjectID;
    using ComponentTypeId	= TypeID;

    static const ObjectID	INVALID_OBJECT_ID	= std::numeric_limits<ObjectID>::max();
    static const TypeID		INVALID_TYPE_ID		= std::numeric_limits<TypeID>::max();
    static const ComponentId INVALID_COMPONENT_ID = std::numeric_limits<ComponentId>::max();
    static const EntityId INVALID_ENTITY_ID = std::numeric_limits<EntityId>::max();


    using SystemTypeId   = TypeID;

    using SystemPriority = u16;


    static const SystemTypeId INVALID_SYSTEMID				= INVALID_TYPE_ID;

    static const SystemPriority LOWEST_SYSTEM_PRIORITY		= std::numeric_limits<SystemPriority>::min();
    static const SystemPriority VERY_LOW_SYSTEM_PRIORITY	= 99;
    static const SystemPriority LOW_SYSTEM_PRIORITY			= 100;
    static const SystemPriority NORMAL_SYSTEM_PRIORITY		= 200;
    static const SystemPriority MEDIUM_SYSTEM_PRIORITY		= 300;
    static const SystemPriority HIGH_SYSTEM_PRIORITY		= 400;
    static const SystemPriority VERY_HIGH_SYSTEM_PRIORITY	= 401;
    static const SystemPriority HIGHEST_SYSTEM_PRIORITY		= std::numeric_limits<SystemPriority>::max();



    union TimeStamp
    {
        f32 asFloat;
        u32 asUInt;

        TimeStamp() : asUInt(0U)
        {}

        TimeStamp(f32 floatValue) : asFloat(floatValue)
        {}

        operator u32() const { return this->asUInt; }

        inline const bool operator==(const TimeStamp& other) const { return this->asUInt == other.asUInt; }
        inline const bool operator!=(const TimeStamp& other) const { return this->asUInt != other.asUInt; }

        inline const bool operator<(const TimeStamp& other) const { return this->asFloat < other.asFloat; }
        inline const bool operator>(const TimeStamp& other) const { return this->asFloat > other.asFloat; }

    }; // union TimeStamp

} // namespace ECS


#endif // __PLATFORM_H__