#pragma once
#include <cstddef>
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <deque>
#include <memory>
#include <algorithm>

namespace detail
{
// Signal
#if 0
template<typename T>
struct signature_traits;

template<typename R, typename... Args>
struct signature_traits<R(Args...)>
{
    template<typename F>
    using is_invocable = std::is_invocable<F, Args...>;

    template<typename F>
    static constexpr bool is_invocable_v = std::is_invocable<F, Args...>::value;
};
#endif

template<typename Signature>
class signal
{
public:
    using size_type = std::size_t;
    using id_type = std::uintptr_t;
    using callback_type = std::function<Signature>;

public:
    id_type connect(callback_type callback)
    {
        slots_.push_back(callback);
        return reinterpret_cast<id_type>(&slots_.back());
    }

    void disconnect(id_type id)
    {
        auto it = std::find_if(
            slots_.begin(),
            slots_.end(),
            [id](const auto& slot)
            {
                return (reinterpret_cast<id_type>(&slot) == id)? true : false;
            }
        );
        if(it != slots_.end())
            slots_.erase(it);
    }

    void emit(const void* event)
    {
        for(auto& slot : slots_)
            slot.operator()(event);
    }

    size_type size() const noexcept{ return slots_.size(); }

private:
    std::deque<callback_type> slots_;
};

class BaseEvent
{
public:
    using Family = std::size_t;

public:
    BaseEvent() = default;
    virtual ~BaseEvent() = default;

protected:
#if defined(__cpp_inline_variables) && (__cpp_inline_variables >= 201606L)
    static inline Family family_counter_ = 0;
#else
    static Family family_counter_;
#endif
};

using EventSignal = signal<void(const void*)>;

class BaseReceiver
{
public:
    virtual ~BaseReceiver()
    {
        for(auto& connection : connections_)
        {
            if(auto sp = connection.second.first.lock())
            {
                sp->disconnect(connection.second.second);
            }
        }
    }

    bool has_connection(BaseEvent::Family family) const
    {
        return connections_.find(family) != connections_.end();
    }

    void connect(BaseEvent::Family family, std::shared_ptr<EventSignal> signal, EventSignal::id_type connection)
    {
        connections_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(family),
            std::forward_as_tuple(signal, connection)
        );
    }

    EventSignal::id_type disconnect(BaseEvent::Family family)
    {
        EventSignal::id_type connection = 0;

        auto it = connections_.find(family);
        if(it != connections_.end())
        {
            connection = it->second.second;
            connections_.erase(it);
        }
        return connection;
    }

private:
    std::unordered_map<
        BaseEvent::Family,
        std::pair<std::weak_ptr<EventSignal>, EventSignal::id_type>
    > connections_;
};

template<typename Event>
struct CallbackWrapper
{
    using callback_type = std::function<void(const Event&)>;

    explicit CallbackWrapper(callback_type callback)
        : callback_(callback)
    {}

    void operator()(const void* event)
    {
        callback_(*static_cast<const Event*>(event));
    }

    callback_type callback_;
};

}   // namespace detail

// Event

template<typename Derived>
class Event
    : public detail::BaseEvent
{
public:
    Event() = default;
    virtual ~Event() = default;

    static detail::BaseEvent::Family family()
    {
        static detail::BaseEvent::Family family = family_counter_++;
        return family;
    }
};

// Receiver

template <typename Derived>
class Receiver
    : public detail::BaseReceiver
{
public:
    Receiver() = default;
    virtual ~Receiver() = default;
};

// EventBus

class EventBus final
{
public:
    EventBus() = default;
    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus& operator = (const EventBus&) = delete;

    EventBus(EventBus&&) = default;
    EventBus& operator = (EventBus&&) = default;

    template<typename Event>
    void publish(const Event& event)
    {
        auto sig = signal_for(Event::family());
        if(sig)
            sig->emit(&event);
    }

    template<typename Event, typename... Args>
    void publish(Args&&... args)
    {
        auto sig = signal_for(Event::family());
        if(sig)
        {
            auto event = Event(std::forward<Args>(args)...);
            sig->emit(&event);
        }
    }

    template<typename Event, typename Receiver>
    void subscribe(Receiver* receiver)
    {
        auto base = static_cast<detail::BaseReceiver*>(receiver);
        if(base->has_connection(Event::family()))
            return;

        void(Receiver::*receive)(const Event&) = &Receiver::receive;
        detail::CallbackWrapper<Event> cbw(std::bind(receive, receiver, std::placeholders::_1));

        auto sig = signal_for(Event::family());
        if(!sig)
        {
            sig = std::make_shared<detail::EventSignal>();
            subscribers_.emplace(Event::family(), sig);
        }

        base->connect(Event::family(), sig, sig->connect(cbw));
    }

    template<typename Event, typename Receiver>
    void unsubscribe(Receiver* receiver)
    {
        auto base = static_cast<detail::BaseReceiver*>(receiver);
        auto connection = base->disconnect(Event::family());
        if(connection == 0)
            return;

        auto it = subscribers_.find(Event::family());
        if(it == subscribers_.end())
            return;

        auto sig = it->second;
        sig->disconnect(connection);

        if(sig->size() == 0)
            subscribers_.erase(it);
    }

    template<typename Event>
    std::size_t connected_receivers() const
    {
        auto it = subscribers_.find(Event::family());
        return (it == subscribers_.end())? 0 : it->second->size();
    }

    std::size_t connected_receivers() const
    {
        std::size_t size = 0;
        for(const auto& subscriber : subscribers_)
        {
            size += subscriber.second->size();
        }
        return size;
    }

private:
    std::shared_ptr<detail::EventSignal> signal_for(detail::BaseEvent::Family family)
    {
        auto it = subscribers_.find(family);
        return (it == subscribers_.end())? nullptr : it->second;
    }

    std::unordered_map<
        detail::BaseEvent::Family,
        std::shared_ptr<detail::EventSignal>
    > subscribers_;
};