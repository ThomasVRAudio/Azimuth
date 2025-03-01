#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorPlayState.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Scripts/ScriptModuleLoader.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Editor/ImGuiStyling.h>

namespace Azimuth
{
    void EditorPlayState::SetPlayState(PlayState playState)
    {
        if (playState == m_PlayState)
            return;

        switch (playState)
        {
        case PlayState::PLAYING:
            StartScene();
            break;
        case PlayState::PAUSED:
            PauseScene();
            break;
        case PlayState::STOPPED:
            StopScene();
            break;
        }

        m_PlayState = playState;
    };

    void EditorPlayState::StartScene()
    {
        if (m_PlayState == PlayState::PAUSED)
        {
            Application::s_PlayingEditorScene = true;
            EditorManager::RenderGizmos(false);
            return;
        }

        Scene *scene = Application::GetActiveScene();
        m_OriginalECS = std::make_unique<ECSManager>(*scene->GetECSManager());
        std::unique_ptr<ECSManager> copy = std::make_unique<ECSManager>(*m_OriginalECS);
        scene->SetECSManager(std::move(copy));
        EditorManager::UpdateLights();
        EditorManager::RenderGizmos(false);

        ScriptModuleLoader::LoadModule();
        ImGuiStyling::SetEditorPlayingStyling();
        Application::s_PlayingEditorScene = true;
    }

    void EditorPlayState::PauseScene()
    {
        EditorManager::RenderGizmos(true);
        Application::s_PlayingEditorScene = false;
    }

    void EditorPlayState::StopScene()
    {
        Scene *scene = Application::GetActiveScene();
        scene->SetECSManager(std::move(m_OriginalECS));
        EditorManager::UpdateLights();
        EditorManager::RenderGizmos(true);
        Application::s_PlayingEditorScene = false;
        ScriptModuleLoader::UnloadModule();
        ImGuiStyling::SetEditorDefaultStyling();
    }
}
#endif