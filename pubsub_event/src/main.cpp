#include <iostream>
#include "event.h"

class FooEvent : public Event<FooEvent>{};
class BarEvent : public Event<BarEvent>{};
class BazEvent : public Event<BazEvent>{};
class QuxEvent : public Event<QuxEvent>{};
class XxxEvent{};

class Listener
    : public Receiver<Listener>
{
public:
    void configure(EventBus& event_bus)
    {
        event_bus.subscribe<FooEvent>(this);
        event_bus.subscribe<BarEvent>(this);
        event_bus.subscribe<BazEvent>(this);
    }

    void unconfigure(EventBus& event_bus)
    {
        event_bus.unsubscribe<FooEvent>(this);
        event_bus.unsubscribe<BarEvent>(this);
        event_bus.unsubscribe<BazEvent>(this);
    }

    void receive(const FooEvent& /* event */)
    {
        std::cout << __func__ << std::endl;
    }

    void receive(const BarEvent& /* event */)
    {
        std::cout << __func__ << std::endl;
    }

    void receive(const BazEvent& /* event */)
    {
        std::cout << __func__ << std::endl;
    }
};

class Foo{};

int main()
{
    EventBus eb;

    std::cout << "test1:" << std::endl;
    {
        Listener listener;
        listener.configure(eb);

        eb.publish(FooEvent());
        eb.publish<BarEvent>();
        eb.publish<BazEvent>();

        listener.unconfigure(eb);
    }
    std::cout << "test2:" << std::endl;
    {
        {
            Listener listener;
            listener.configure(eb);
        }
        eb.publish(FooEvent());
        eb.publish(BarEvent());
        eb.publish(BazEvent());
    }
    std::cout << "test3:" << std::endl;
    {
        Listener listener;
        eb.subscribe<FooEvent>(&listener);

        eb.publish(FooEvent());
    }
}
