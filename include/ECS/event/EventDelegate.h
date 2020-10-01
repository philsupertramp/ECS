//
// Created by phil on 01.10.20.
//

#ifndef ECS_EVENTDELEGATE_H
#define ECS_EVENTDELEGATE_H

#include "../Platform.h"
#include "IEventDelegate.h"

namespace ECS::Event {

    template<class Class, class EventType>
    class EventDelegate : public IEventDelegate
    {
        typedef void(Class::*Callback)(const EventType* const);

        Class* m_Receiver;
        Callback m_Callback;

    public:

        EventDelegate(Class* receiver, Callback& callbackFunction) :
                m_Receiver(receiver),
                m_Callback(callbackFunction)
        {}

        virtual IEventDelegate* clone() override {
            return new EventDelegate(this->m_Receiver, this->m_Callback);
        }

        virtual inline void invoke(const IEvent* const e) override {
            (m_Receiver->*m_Callback)(reinterpret_cast<const EventType* const>(e));
        }

        virtual inline EventDelegateId GetDelegateId() const override {
            static const EventDelegateId DELEGATE_ID { typeid(Class).hash_code() ^ typeid(Callback).hash_code() };
            return DELEGATE_ID;
        }

        virtual inline u64 GetStaticEventTypeId() const override {
            static const u64 SEID { EventType::STATIC_EVENT_TYPE_ID };
            return SEID;
        }

        virtual bool operator==(const IEventDelegate* other) const override {
            if (this->GetDelegateId() != other->GetDelegateId())
                return false;

            auto delegate = (EventDelegate*)other;
            if (delegate == nullptr)
                return false;

            return ((this->m_Callback == delegate->m_Callback) && (this->m_Receiver == delegate->m_Receiver));
        }

        void Subscribe(){
        }
        void Unsubscribe(){
        }

    }; // class EventDelegate
}


#endif //ECS_EVENTDELEGATE_H
