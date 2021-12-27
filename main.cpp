#include <iostream>

#include "Clock.hpp"
#include "EventsStatistic.hpp"

class TestClock: public Clock {
public:
    TestClock(TimePoint now = TimePoint{}) : _now{now} {}
    void set(TimePoint newPoint) { _now = newPoint; }
    void advance(Duration dur) { _now += dur; }

    TimePoint now() override { return _now; }

private:
    TimePoint _now;
};

int main() {
    std::shared_ptr<TestClock> clock = std::make_shared<TestClock>();
    EventsStatisticWithClock stat{clock};
    for (size_t i = 0; i < 60; ++i) {
        stat.incEvent("1");
        if (i % 2 == 0) {
            stat.incEvent("2");
        }
        clock->advance(std::chrono::minutes{1});
    }

    std::cout << stat.getEventStatisticByName("1") << std::endl;
    std::cout << stat.getEventStatisticByName("2") << std::endl;
    std::cout << stat.getEventStatisticByName("3") << std::endl << std::endl;
    stat.printStatistic();

    clock->advance(std::chrono::minutes{30});

    std::cout << stat.getEventStatisticByName("1") << std::endl;
    std::cout << stat.getEventStatisticByName("2") << std::endl;
    std::cout << stat.getEventStatisticByName("3") << std::endl << std::endl;
    stat.printStatistic();
}
