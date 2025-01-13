#include <Azimuth/Core/Time.h>

namespace Azimuth
{
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_StartTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_LastTimeStep;
}