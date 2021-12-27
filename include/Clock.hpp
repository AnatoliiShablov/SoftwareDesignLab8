#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>
#include <memory>

class Clock: public std::enable_shared_from_this<Clock> {
public:
    using UnderlyingClock = std::chrono::high_resolution_clock;
    using TimePoint       = UnderlyingClock::time_point;
    using Duration        = UnderlyingClock::duration;
    [[nodiscard]] virtual TimePoint now();
    virtual ~Clock();
};

#endif  // CLOCK_HPP
