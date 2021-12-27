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
    _events.emplace_back(this, name);
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
    std::erase_if(_events, [&clock = _clock, duration = _statisticDuration](Event const& e) {
        return e.timePassed(clock->now()) > duration;
    });
}

EventsStatisticWithClock::Event::Event(EventsStatisticWithClock* stat, std::string_view name)
    : _stat{stat}, _name{name}, startTp{_stat->_clock->now()} {
    auto it = _stat->_counter.find(_name);
    if (it == _stat->_counter.end()) {
        it    = _stat->_counter.emplace(_name, EventCounter{}).first;
        _name = it->first;
    }
    it->second.addToPeriod();
}

Clock::Duration EventsStatisticWithClock::Event::timePassed(Clock::TimePoint now) const noexcept {
    return now - startTp;
}

EventsStatisticWithClock::Event::~Event() noexcept(false) {
    auto it = _stat->_counter.find(_name);
    if (it == _stat->_counter.end()) {
        throw std::runtime_error("Removing unexisting element");
    }
    it->second.removeFromPeriod();
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
