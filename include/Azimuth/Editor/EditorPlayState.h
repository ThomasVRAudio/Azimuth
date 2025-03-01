#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/PlayState.h>
#include <Azimuth/ECS/ECSManager.h>

namespace Azimuth
{
    class EditorPlayState
    {
    public:
        inline static PlayState GetPlayState() { return m_PlayState; };
        static void SetPlayState(PlayState playState);

    private:
        inline static std::unique_ptr<ECSManager> m_OriginalECS = nullptr;
        static void StartScene();
        static void PauseScene();
        static void StopScene();
        inline static PlayState m_PlayState = PlayState::STOPPED;
    };
}
#endif