#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <event.h>

namespace
{

class EventTest : public ::testing::Test {};

class FooEvent : public Event<FooEvent> {};
class BarEvent : public Event<BarEvent> {};
class BazEvent : public Event<BarEvent> {};

class Listener
    : public Receiver<Listener>
{
public:
    Listener()
        : foo_counter_(0)
        , bar_counter_(0)
    {}

    void receive(const FooEvent& event)
    {
        ++foo_counter_;
    }

    void receive(const BarEvent& event)
    {
        ++bar_counter_;
    }

    int foo_counter() const { return foo_counter_; }
    int& foo_counter(){ return foo_counter_; }

    int bar_counter() const { return bar_counter_; }
    int& bar_counter() { return bar_counter_; }

private:
    int foo_counter_;
    int bar_counter_;
};

TEST_F(EventTest, publish)
{
    EventBus eb;

    Listener listener;

    eb.subscribe<FooEvent>(&listener);
    eb.subscribe<BarEvent>(&listener);

    EXPECT_EQ(0, listener.foo_counter());
    EXPECT_EQ(0, listener.bar_counter());

    eb.publish<FooEvent>();
    eb.publish<BarEvent>();

    EXPECT_EQ(1, listener.foo_counter());
    EXPECT_EQ(1, listener.bar_counter());

    eb.publish<FooEvent>();

    EXPECT_EQ(2, listener.foo_counter());
    EXPECT_EQ(1, listener.bar_counter());

    eb.unsubscribe<FooEvent>(&listener);
    eb.publish(FooEvent());
    eb.publish(BarEvent());

    EXPECT_EQ(2, listener.foo_counter());
    EXPECT_EQ(2, listener.bar_counter());
}

TEST_F(EventTest, connected_receivers)
{
    {
        EventBus eb;
        Listener listener;

        EXPECT_EQ(0, eb.connected_receivers<FooEvent>());
        EXPECT_EQ(0, eb.connected_receivers<BarEvent>());
        EXPECT_EQ(0, eb.connected_receivers());

        eb.subscribe<FooEvent>(&listener);
        eb.subscribe<BarEvent>(&listener);

        EXPECT_EQ(1, eb.connected_receivers<FooEvent>());
        EXPECT_EQ(1, eb.connected_receivers<BarEvent>());
        EXPECT_EQ(2, eb.connected_receivers());

        eb.unsubscribe<BarEvent>(&listener);

        EXPECT_EQ(1, eb.connected_receivers<FooEvent>());
        EXPECT_EQ(0, eb.connected_receivers<BarEvent>());
        EXPECT_EQ(1, eb.connected_receivers());

        eb.unsubscribe<FooEvent>(&listener);

        EXPECT_EQ(0, eb.connected_receivers<FooEvent>());
        EXPECT_EQ(0, eb.connected_receivers<BarEvent>());
        EXPECT_EQ(0, eb.connected_receivers());
    }
    {
        EventBus eb;
        {
            Listener listener;

            EXPECT_EQ(0, eb.connected_receivers<FooEvent>());
            EXPECT_EQ(0, eb.connected_receivers<BarEvent>());
            EXPECT_EQ(0, eb.connected_receivers());

            eb.subscribe<FooEvent>(&listener);
            eb.subscribe<BarEvent>(&listener);

            EXPECT_EQ(1, eb.connected_receivers<FooEvent>());
            EXPECT_EQ(1, eb.connected_receivers<BarEvent>());
            EXPECT_EQ(2, eb.connected_receivers());
        }
        EXPECT_EQ(0, eb.connected_receivers<FooEvent>());
        EXPECT_EQ(0, eb.connected_receivers<BarEvent>());
        EXPECT_EQ(0, eb.connected_receivers());
    }
}

}   // namespace