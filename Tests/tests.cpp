#include "Clock.hpp"
#include "EventsStatistic.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class TestClock: public Clock {
public:
    TestClock(TimePoint now = TimePoint{}) : _now{now} {}
    void set(TimePoint newPoint) { _now = newPoint; }
    void advance(Duration dur) { _now += dur; }

    TimePoint now() override { return _now; }

private:
    TimePoint _now;
};

constexpr std::string_view AnyName{"AnyName"};
constexpr std::string_view NotExists{"NotExists"};
constexpr std::string_view Exists{"Exists"};

TEST(Simple, OneEvent) {
    std::shared_ptr<TestClock> clock = std::make_shared<TestClock>();
    EventsStatisticWithClock stat{clock};

    ASSERT_TRUE(stat.getAllEventStatistic().empty());
    ASSERT_EQ(stat.getEventStatisticByName(AnyName), 0);

    stat.incEvent(Exists);

    for (size_t i{0}; i < 60; ++i) {
        ASSERT_FALSE(stat.getAllEventStatistic().empty());
        ASSERT_FLOAT_EQ(stat.getEventStatisticByName(Exists), 1 / 60.0);
        ASSERT_FLOAT_EQ(stat.getEventStatisticByName(NotExists), 0);

        clock->advance(std::chrono::minutes{1});
    }
    clock->advance(std::chrono::minutes{1});

    ASSERT_TRUE(stat.getAllEventStatistic().empty());
    ASSERT_EQ(stat.getEventStatisticByName(Exists), 0);
}

TEST(Simple, TwoEvents) {
    std::shared_ptr<TestClock> clock = std::make_shared<TestClock>();
    EventsStatisticWithClock stat{clock};

    stat.incEvent(NotExists);
    ASSERT_FLOAT_EQ(stat.getEventStatisticByName(NotExists), 1 / 60.0);
    clock->advance(std::chrono::minutes{30});
    ASSERT_FLOAT_EQ(stat.getEventStatisticByName(NotExists), 1 / 60.0);
    stat.incEvent(Exists);
    ASSERT_FLOAT_EQ(stat.getEventStatisticByName(NotExists), 1 / 60.0);
    ASSERT_FLOAT_EQ(stat.getEventStatisticByName(Exists), 1 / 60.0);
    clock->advance(std::chrono::minutes{31});

    ASSERT_FLOAT_EQ(stat.getEventStatisticByName(NotExists), 0);
    ASSERT_FLOAT_EQ(stat.getEventStatisticByName(Exists), 1 / 60.0);
}

TEST(MoreEvents, OneEventMoreThanOnce) {
    std::shared_ptr<TestClock> clock = std::make_shared<TestClock>();
    EventsStatisticWithClock stat{clock};

    for (size_t i{0}; i < 60; ++i) {
        ASSERT_FLOAT_EQ(stat.getEventStatisticByName(Exists), i / 60.0);
        stat.incEvent(Exists);
        clock->advance(std::chrono::minutes{1});
    }

    for (size_t i{0}; i < 60; ++i) {
        ASSERT_FLOAT_EQ(stat.getEventStatisticByName(Exists), (60 - i) / 60.0);
        clock->advance(std::chrono::minutes{1});
    }
}

TEST(PrintTest, JustSimpleTest) {
    std::shared_ptr<TestClock> clock = std::make_shared<TestClock>();
    EventsStatisticWithClock stat{clock};

    stat.incEvent("1");
    stat.incEvent("1");
    stat.incEvent("2");

    struct T {
        T() : s{}, prev{std::cout.rdbuf(s.rdbuf())} {}

        std::string getCurrent() { return s.str(); }

        ~T() { std::cout.rdbuf(prev); }

    private:
        std::stringstream s;
        std::streambuf* prev;
    } t;

    stat.printStatistic();

    ASSERT_EQ(t.getCurrent(),
              "1:\t2\n"
              "2:\t1\n\n");
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
