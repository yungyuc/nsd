#include <cstdint>

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

    mach_timebase_info_data_t m_tbinfo;
    uint64_t m_start = 0;
    uint64_t m_end = 0;

}; /* end struct StopWatch */

#elif __linux__

struct StopWatch
{
    double lap() { return 0; /* TODO */ }
}; /* end struct StopWatch */

#endif // __APPLE__
