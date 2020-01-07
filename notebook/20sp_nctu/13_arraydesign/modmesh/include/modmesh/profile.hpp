#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD-style license; see COPYING
 */

#include "modmesh/base.hpp"

/*
 * MODMESH_PROFILE defined: Enable profiling API.
 */
#ifdef MODMESH_PROFILE

#define MODMESH_TIME(NAME) \
    ScopedTimer local_scoped_timer_ ## __LINE__(NAME);

/*
 * No MODMESH_PROFILE defined: Disable profiling API.
 */
#else // MODMESH_PROFILE

#define MODMESH_TIME(NAME)

#endif // MODMESH_PROFILE
/*
 * End MODMESH_PROFILE.
 */

namespace modmesh
{

#if __APPLE__

#include <assert.h>
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

struct StopWatch
{

    StopWatch()
    {
        mach_timebase_info(&m_tbinfo);
        lap();
    }

    double lap()
    {
        m_start = m_end;
        m_end = mach_absolute_time();
        uint64_t elapsed = m_end - m_start;
        return elapsed * m_tbinfo.numer / m_tbinfo.denom * 1.e-9;
    }

    /// A global singleton.
    static StopWatch & me()
    {
        static StopWatch instance;
        return instance;
    }

    mach_timebase_info_data_t m_tbinfo;
    uint64_t m_start = 0;
    uint64_t m_end = 0;

}; /* end struct StopWatch */

#elif __linux__

#include <time.h>

struct StopWatch
{
    StopWatch()
    {
        clock_getres(CLOCK_PROCESS_CPUTIME_ID, &m_res);
        lap();
    }

    double lap()
    {
        m_start = m_end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_end);
        return diff(m_start, m_end) * 1.e-9;
    }

    int64_t diff(timespec const & t1, timespec const & t2)
    {
        timespec temp;
        int64_t nsec;
        if ((t2.tv_nsec - t1.tv_nsec)<0)
        {
            nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
            nsec += 1000000000 * (t2.tv_sec - t1.tv_sec - 1);
        }
        else
        {
            nsec = t2.tv_nsec - t1.tv_nsec;
            nsec += 1000000000 * (t2.tv_sec - t1.tv_sec);
        }
        return nsec;
    }

    /// A global singleton.
    static StopWatch & me()
    {
        static StopWatch instance;
        return instance;
    }

    timespec m_res;
    timespec m_start;
    timespec m_end;

}; /* end struct StopWatch */

#endif

struct TimedEntry
{
    size_t m_count = 0;
    double m_time = 0.0;
}; /* end struct TimedEntry */

class TimeRegistry
{

public:

    static TimeRegistry & me()
    {
        static TimeRegistry inst;
        return inst;
    }

    std::string report() const
    {
        std::ostringstream ostm;
        for (auto it = m_entry.begin() ; it != m_entry.end() ; ++it)
        {
            ostm
                << it->first << " : "
                << "count = " << it->second.m_count << " , "
                << "time = " << it->second.m_time << " (second)"
                << std::endl;
        }
        return ostm.str();
    }

    void add(const char * name, double time)
    {
        auto it = m_entry.find(name);
        if (it == m_entry.end())
        {
            it = std::get<0>(m_entry.insert({name, {0, 0.0}}));
        }
        ++(it->second.m_count);
        it->second.m_time += time;
    }

    ~TimeRegistry()
    {
        // Uncomment for debugging.
        //std::cout << report();
    }

private:

    TimeRegistry() = default;
    TimeRegistry(TimeRegistry const & ) = delete;
    TimeRegistry(TimeRegistry       &&) = delete;
    TimeRegistry & operator=(TimeRegistry const & ) = delete;
    TimeRegistry & operator=(TimeRegistry       &&) = delete;

    std::map<const char *, TimedEntry> m_entry;

}; /* end struct TimeRegistry */

struct ScopedTimer
{

    ScopedTimer() = delete;

    ScopedTimer(const char * name) : m_name(name) {}

    ~ScopedTimer()
    {
        TimeRegistry::me().add(m_name, m_sw.lap());
    }

    StopWatch m_sw;
    char const * m_name;

}; /* end struct ScopedTimer */

} /* end namespace modmesh */

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
