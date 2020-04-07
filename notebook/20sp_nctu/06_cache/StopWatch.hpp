#pragma once

#include <chrono>

class StopWatch
{

private:

    using clock_type = std::chrono::high_resolution_clock;
    using time_type = std::chrono::time_point<clock_type>;

public:

    /// A singleton.
    static StopWatch & me()
    {
        static StopWatch instance;
        return instance;
    }

    StopWatch() : m_start(clock_type::now()), m_stop(m_start) {}

    StopWatch(StopWatch const & ) = default;
    StopWatch(StopWatch       &&) = default;
    StopWatch & operator=(StopWatch const & ) = default;
    StopWatch & operator=(StopWatch       &&) = default;
    ~StopWatch() = default;

    /**
     * Return seconds between laps.
     */
    double lap()
    {
        m_start = m_stop;
        m_stop = clock_type::now();
        return std::chrono::duration<double>(m_stop - m_start).count();
    }

    /**
     * Return seconds between end and start.
     */
    double duration() const { return std::chrono::duration<double>(m_stop - m_start).count(); }

    /**
     * Return resolution in second.
     */
    static constexpr double resolution()
    {
        return double(clock_type::period::num) / double(clock_type::period::den);
    }

private:

    time_type m_start;
    time_type m_stop;

}; /* end struct StopWatch */

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
