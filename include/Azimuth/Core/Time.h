#pragma once
#include <chrono>

namespace Azimuth
{

    class Application;
    class Time
    {
    public:
        static double time()
        {
            auto time = std::chrono::high_resolution_clock::now() - m_StartTime;
            return std::chrono::duration<double>(time).count();
        }
        static double deltaTime()
        {
            auto time = std::chrono::high_resolution_clock::now() - m_LastTimeStep;
            m_LastTimeStep += time;
            return std::chrono::duration<double>(time).count();
        }

    private:
        static std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        static void StartGlobalTime()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        static void AddTimeStep()
        {
            m_LastTimeStep = std::chrono::high_resolution_clock::now();
        }
        static std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTimeStep;
        friend Application;
    };
}