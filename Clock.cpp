#include "Clock.hpp"

Clock::TimePoint Clock::now() {
    return UnderlyingClock::now();
}

Clock::~Clock() = default;
