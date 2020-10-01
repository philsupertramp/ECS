//
// Created by phil on 01.10.20.
//

#ifndef ECS_IEVENTLISTENER_H
#define ECS_IEVENTLISTENER_H

#include "../Platform.h"
#include "EventDelegate.h"

namespace ECS::Event {

    class IEventListener {
        friend class ECSEngine;

        using RegisteredCallbacks = std::list<IEventDelegate*>;
        RegisteredCallbacks m_RegisteredCallbacks;
    public:

        IEventListener();
        virtual ~IEventListener();
        template<class E, class C>
        inline void RegisterEventCallback(void(C::*Callback)(const E* const))
        {

            IEventDelegate* eventDelegate = new EventDelegate<C, E>(static_cast<C*>(this), Callback);

            m_RegisteredCallbacks.push_back(eventDelegate);
        }
        template<class E, class C>
        inline void UnregisterEventCallback(void(C::*Callback)(const E* const))
        {
            EventDelegate<C, E> delegate(static_cast<C*>(this), Callback);

            for (auto cb : this->m_RegisteredCallbacks)
            {
                if (cb->GetDelegateId() == delegate.GetDelegateId())
                {
                    this->m_RegisteredCallbacks.remove_if(
                            [&](const IEventDelegate* other)
                            {
                                return other->operator==(cb);
                            }
                    );

                    break;
                }
            }
        }

        void UnregisterAllEventCallbacks();
    };
}


#endif //ECS_IEVENTLISTENER_H
