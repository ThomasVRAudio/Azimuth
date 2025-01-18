#pragma once
#include <Azimuth/Events/Event.h>

namespace Azimuth
{

    class MouseScrollEvent : public Event<double, double>
    {
    };

    class MouseCursorEvent : public Event<double, double>
    {
    };

    inline MouseScrollEvent g_ScrollEvent;
    inline MouseCursorEvent g_CursorEvent;
}