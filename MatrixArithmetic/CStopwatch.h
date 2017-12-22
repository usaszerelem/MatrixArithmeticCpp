#pragma once

#include <time.h>


class CStopwatch
{
public:
    inline CStopwatch() { m_Time = 0; }
    inline void Start() { m_Time = clock(); }

    // warning C4244: 'return': conversion from 'clock_t' to 'float', possible loss of data
#pragma warning( push )
#pragma warning( disable : 4244)
    inline float Stop() { return((clock() - m_Time) / CLOCKS_PER_SEC); }
#pragma warning( pop ) 

private:
    clock_t m_Time;
};