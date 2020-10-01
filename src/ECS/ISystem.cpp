//
// Created by phil on 01.10.20.
//

#include "../../include/ECS/system/ISystem.h"

namespace ECS {

    ISystem::ISystem(SystemPriority priority, f32 updateInterval_ms)
    : m_Priority(priority),
      m_UpdateInterval(updateInterval_ms),
      m_Enabled(true)
    {}

    ISystem::~ISystem()
    {}
}