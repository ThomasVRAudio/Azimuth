#pragma once
#include <Azimuth/Editor/PlayState.h>

namespace Azimuth
{

    class EditorSceneControlPanel
    {
    public:
        static void DrawPanel();

    private:
        inline static PlayState m_PlayState = PlayState::STOPPED;
    };
}