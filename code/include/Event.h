#pragma once

#include <atomic>
#include <unordered_map>

#include "Container/Vector.h"

namespace nxt::core {

class BaseEvent {
protected:
    static std::atomic<uint32_t> next_id_;
};

template<class TEvent>
class Event : public BaseEvent {
public:
    static uint32_t GetEventId() {
        return event_id_;
    }

private:
    static uint32_t event_id_;
};

template<class TEvent>
uint32_t Event<TEvent>::event_id_ = BaseEvent::next_id_++;

class BaseReceiver {
public:
    virtual ~BaseReceiver() = default;
};

template<class TEventData>
class Receiver : public BaseReceiver {
public:
    virtual void Receive(const TEventData&) = 0;
};

class EventSource {
public:
    template<typename TEvent>
    void Subscribe(Receiver<TEvent>* receiver) {
        auto event_id = TEvent::GetEventId();
        auto& receivers = subscribers_[event_id];
        receivers.pushBack(receiver);
    }

    template<typename TEvent>
    void Unsubscribe(Receiver<TEvent>* receiver) {
        auto iter = subscribers_.find(TEvent::GetEventId());
        if (iter != subscribers_.end()) {
            auto& receivers = iter->second;
            auto receiver_iter = receivers.begin();
            while (receiver_iter != receivers.end()) {
                if (*receiver_iter == receiver) {
                    receivers.erase(receiver_iter);
                    break;
                }
            }
        }
    }

protected:
    template<typename TEvent>
    void Emit(const TEvent& event) {
        auto& receivers = subscribers_[TEvent::GetEventId()];
        for (auto& receiver : receivers) {
            static_cast<Receiver<TEvent>*>(receiver)->Receive(event);
        }
    }

private:
    std::unordered_map<uint32_t, Vector<BaseReceiver*>> subscribers_;
};
}  // namespace next::core
