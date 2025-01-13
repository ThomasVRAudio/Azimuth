#pragma once
#include <chrono>

namespace Azimuth
{

    class Application;
    class Time
    {
    public:
        static float time()
        {
            auto time = std::chrono::high_resolution_clock::now() - m_StartTime;
            return std::chrono::duration<float>(time).count();
        }
        static float deltaTime()
        {
            auto time = std::chrono::high_resolution_clock::now() - m_LastTimeStep;
            m_LastTimeStep += time;
            return std::chrono::duration<float>(time).count();
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