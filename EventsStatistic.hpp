#ifndef EVENTSSTATISTIC_HPP
#define EVENTSSTATISTIC_HPP

#include <deque>
#include <map>
#include <string_view>

#include "Clock.hpp"

class EventsStatistic {
public:
    EventsStatistic(Clock::Duration statisticDuration = std::chrono::hours{1}) noexcept;
    virtual void incEvent(std::string_view name)                                           = 0;
    [[nodiscard]] virtual float getEventStatisticByName(std::string_view name)             = 0;
    [[nodiscard]] virtual std::map<std::string, float, std::less<>> getAllEventStatistic() = 0;
    virtual void printStatistic()                                                          = 0;

    virtual ~EventsStatistic();

protected:
    Clock::Duration _statisticDuration;
};

class EventsStatisticWithClock: public EventsStatistic {
public:
    EventsStatisticWithClock(std::shared_ptr<Clock> clock,
                             Clock::Duration statisticDuration = std::chrono::hours{1}) noexcept;

    void incEvent(std::string_view name) override;

    float getEventStatisticByName(std::string_view name) override;

    std::map<std::string, float, std::less<>> getAllEventStatistic() override;
    void printStatistic() override;

private:
    friend class Event;

    class Event {
    public:
        Event(EventsStatisticWithClock* stat, std::string_view name);
        [[nodiscard]] Clock::Duration timePassed(Clock::TimePoint now) const noexcept;

        ~Event() noexcept(false);

    private:
        EventsStatisticWithClock* _stat;
        std::string_view _name;
        Clock::TimePoint startTp;
    };

    class EventCounter {
    public:
        EventCounter();
        uint64_t getAllStat() const noexcept;
        uint64_t getPeriodStat() const noexcept;
        void addToPeriod() noexcept;
        void removeFromPeriod();

    private:
        uint64_t _all;
        uint64_t _period;
    };

    void cleanOld();

    std::deque<Event> _events;
    std::map<std::string, EventCounter, std::less<>> _counter;
    std::shared_ptr<Clock> _clock;
    float _den;
};
#endif  // EVENTSSTATISTIC_HPP
