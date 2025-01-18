#pragma once
#include <Azimuth/Events/Event.h>
#include <Azimuth/Renderer/Window.h>

namespace Azimuth
{

    class MouseScrollEvent : public Event<double, double>
    {
    public:
        static void InitializeCallbacks();
    };

    class MouseCursorEvent : public Event<double, double>
    {
    public:
        static void InitializeCallbacks();
    };

    inline MouseScrollEvent g_ScrollEvent;
    inline MouseCursorEvent g_CursorEvent;
}
