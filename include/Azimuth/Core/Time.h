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

        inline static double deltaTime() { return m_DeltaTime; }

    private:
        static void StartGlobalTime()
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
            m_LastTimeStep = m_StartTime;
        }

        static void AddTimeStep()
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> delta = currentTime - m_LastTimeStep;
            m_DeltaTime = delta.count();
            m_LastTimeStep = currentTime;
        }
        inline static double m_DeltaTime;
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTimeStep;
        friend Application;
    };
}