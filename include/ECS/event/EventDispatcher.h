//
// Created by phil on 01.10.20.
//

#ifndef ECS_EVENTDISPATCHER_H
#define ECS_EVENTDISPATCHER_H

#include "IEventDispatcher.h"

namespace ECS::Event {
    template<class T>
    class EventDispatcher : public IEventDispatcher {
        using EventDelegateList = std::list<IEventDelegate *>;

        //using PendingAddDelegates		= std::list<IEventDelegate*>;
        using PendingRemoveDelegates = std::list<IEventDelegate *>;

        //PendingAddDelegates		m_PendingAddDelegates;
        PendingRemoveDelegates m_PendingRemoveDelegates;

        EventDelegateList m_EventCallbacks;

        bool m_Locked;

    public:

        // never use!
        EventDispatcher() :
                m_Locked(false) {}

        virtual ~EventDispatcher() {
            //this->m_PendingAddDelegates.clear();
            this->m_PendingRemoveDelegates.clear();
            this->m_EventCallbacks.clear();
        }

        // send event to all listener
        inline void Dispatch(IEvent *event) override {
            this->m_Locked = true;
            {
//                Fawxy::Log::ECS->Trace("Dispatch event " + std::string(typeid(T).name()));

                // remove pending delegates
                if (this->m_PendingRemoveDelegates.empty() == false) {
                    for (auto EC : this->m_PendingRemoveDelegates) {
                        auto result = std::find_if(this->m_EventCallbacks.begin(), this->m_EventCallbacks.end(),
                                                   [&](const IEventDelegate *other) {
                                                       return other->operator==(EC);
                                                   });

                        if (result != this->m_EventCallbacks.end()) {
                            IEventDelegate *ptrMem = (IEventDelegate *) (*result);

                            this->m_EventCallbacks.erase(result);

                            delete ptrMem;
                            ptrMem = nullptr;
                        }
                    }
                    this->m_PendingRemoveDelegates.clear();
                }

                for (auto EC : this->m_EventCallbacks) {
                    assert(EC != nullptr && "Invalid event callback.");
                    EC->invoke(event);
                }
            }
            this->m_Locked = false;
        }

        virtual void AddEventCallback(IEventDelegate *const eventDelegate) override {
            // if delegate wasn't deleted since last update, that is, delegate is still in pending list,
            // remove it from pending list
            auto result = std::find_if(this->m_PendingRemoveDelegates.begin(), this->m_PendingRemoveDelegates.end(),
                                       [&](const IEventDelegate *other) {
                                           return other->operator==(eventDelegate);
                                       });

            if (result != this->m_PendingRemoveDelegates.end()) {
                this->m_PendingRemoveDelegates.erase(result);
                return;
            }

            this->m_EventCallbacks.push_back(eventDelegate);
        }

        virtual void RemoveEventCallback(IEventDelegate *eventDelegate) override {
            if (this->m_Locked == false) {
                auto result = std::find_if(this->m_EventCallbacks.begin(), this->m_EventCallbacks.end(),
                                           [&](const IEventDelegate *other) {
                                               return other->operator==(eventDelegate);
                                           });

                if (result != this->m_EventCallbacks.end()) {
                    IEventDelegate *ptrMem = (IEventDelegate *) (*result);

                    this->m_EventCallbacks.erase(result);

                    delete ptrMem;
                    ptrMem = nullptr;
                }
            } else {
                auto result = std::find_if(this->m_EventCallbacks.begin(), this->m_EventCallbacks.end(),
                                           [&](const IEventDelegate *other) {
                                               return other->operator==(eventDelegate);
                                           });

                assert(result != this->m_EventCallbacks.end() && "");
                this->m_PendingRemoveDelegates.push_back((*result));
            }
        }

        virtual inline size_t GetEventCallbackCount() const override { return this->m_EventCallbacks.size(); }
    };
}

#endif //ECS_EVENTDISPATCHER_H
