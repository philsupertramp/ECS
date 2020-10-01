//
// Created by phil on 01.10.20.
//

#ifndef ECS_ISYSTEM_H
#define ECS_ISYSTEM_H

#include "../Platform.h"

namespace ECS {

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


    class ISystem
    {

    private:

        /// Summary:	Duration since last system update in milliseconds.
        f32						m_TimeSinceLastUpdate;

        SystemPriority			m_Priority;

        /// Summary:	The system update interval.
        /// A negative value means system should update each time the engine receives an update.
        f32						m_UpdateInterval;

        u8						m_Enabled		: 1;
        u8						m_NeedsUpdate	: 1;
        u8						m_Reserved		: 6;

        ISystem(SystemPriority priority = NORMAL_SYSTEM_PRIORITY, f32 updateInterval_ms = -1.0f);

    public:

        virtual ~ISystem();

        /**
         * Getter for static Type ID
         * @return
         */
        virtual inline const SystemTypeId GetStaticSystemTypeID() const = 0;
        /**
         * Getter for static type name
         * @return
         */
        virtual inline const char* GetSystemTypeName() const = 0;

        /**
         * pre update step
         * @param dt
         */
        virtual void PreUpdate(f32 dt)	= 0;

        /**
         * update step
         *
         * Interface for System update.
         * In most cases a system needs to be updated every iteration of the game loop.
         * The system update is split into 3 phases, pre-update; update and post-update
         * to perform necessary steps
         * @param dt
         */
        virtual void Update(f32 dt)		= 0;

        /**
         * post update step
         * @param dt
         */
        virtual void PostUpdate(f32 dt) = 0;
    };
}

#endif //ECS_ISYSTEM_H
