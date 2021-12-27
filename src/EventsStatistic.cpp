#include "EventsStatistic.hpp"

#include <iostream>

EventsStatistic::EventsStatistic(Clock::Duration statisticDuration) noexcept : _statisticDuration{statisticDuration} {}

EventsStatistic::~EventsStatistic() = default;

EventsStatisticWithClock::EventsStatisticWithClock(std::shared_ptr<Clock> clock,
                                                   Clock::Duration statisticDuration) noexcept
    : EventsStatistic{statisticDuration}
    , _clock{std::move(clock)}
    , _den{
          std::chrono::duration_cast<std::chrono::duration<float, Clock::Duration::period>>(statisticDuration).count() /
          std::chrono::duration_cast<std::chrono::duration<float, Clock::Duration::period>>(std::chrono::minutes{1})
              .count()} {}

void EventsStatisticWithClock::incEvent(std::string_view name) {
    cleanOld();
    auto it = _counter.find(name);
    if (it == _counter.end()) {
        it = _counter.emplace(name, EventCounter{}).first;
    }
    _events.emplace_back(_clock->now(), it->first);
    it->second.addToPeriod();
}

float EventsStatisticWithClock::getEventStatisticByName(std::string_view name) {
    cleanOld();
    if (auto it = _counter.find(name); it != _counter.end()) {
        return it->second.getPeriodStat() / _den;
    }
    return 0;
}

std::map<std::string, float, std::less<>> EventsStatisticWithClock::getAllEventStatistic() {
    std::map<std::string, float, std::less<>> result;
    cleanOld();
    for (auto const& [k, v] : _counter) {
        if (v.getPeriodStat() > 0) {
            result.emplace(k, v.getPeriodStat() / _den);
        }
    }
    return result;
}

void EventsStatisticWithClock::printStatistic() {
    cleanOld();
    for (auto const& [k, v] : _counter) {
        std::cout << k << ":\t" << v.getAllStat() << '\n';
    }
    std::cout << std::endl;
}

void EventsStatisticWithClock::cleanOld() {
    while (!_events.empty() && _events.front().timePassed(_clock->now()) > _statisticDuration) {
        _counter.find(_events.front().name())->second.removeFromPeriod();
        _events.pop_front();
    }
}

EventsStatisticWithClock::Event::Event(Clock::TimePoint tp, std::string_view name) : _name{name}, startTp{tp} {}

std::string_view EventsStatisticWithClock::Event::name() const noexcept {
    return _name;
}

Clock::Duration EventsStatisticWithClock::Event::timePassed(Clock::TimePoint now) const noexcept {
    return now - startTp;
}

EventsStatisticWithClock::EventCounter::EventCounter() : _all{0}, _period{0} {}

uint64_t EventsStatisticWithClock::EventCounter::getAllStat() const noexcept {
    return _all;
}

uint64_t EventsStatisticWithClock::EventCounter::getPeriodStat() const noexcept {
    return _period;
}

void EventsStatisticWithClock::EventCounter::addToPeriod() noexcept {
    ++_all;
    ++_period;
}

void EventsStatisticWithClock::EventCounter::removeFromPeriod() {
    if (_period == 0) {
        throw std::runtime_error("No events in period");
    }
    --_period;
}
