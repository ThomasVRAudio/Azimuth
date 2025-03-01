#include <Azimuth/Scripts/ScriptLayer.h>
#include <Azimuth/ECS/Components/ScriptContainerComponent.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Common.h>

namespace Azimuth
{

    void ScriptLayer::Init(Scene *scene)
    {
        m_Scene = scene;
        ECSManager *ECS = scene->GetECSManager();

        ComponentMask mask;

        mask.set(ECS->GetComponentBitType<ScriptContainerComponent>(), true);
        m_ScriptSystem = ECS->RegisterSystem<ScriptSystem>();
        ECS->SetSystemComponentMask<ScriptSystem>(mask);
        m_ScriptSystem->Init(scene);
    }

    void ScriptLayer::OnStart()
    {
#ifdef AZIMUTH_EDITOR
        if (!Application::s_PlayingEditorScene)
            return;
#endif

        m_ScriptSystem->OnStart();
    }

    void ScriptLayer::OnUpdate()
    {
#ifdef AZIMUTH_EDITOR
        if (!Application::s_PlayingEditorScene)
            return;

        if (Application::s_IsFirstPlayFrame)
        {
            OnStart();
            Application::s_IsFirstPlayFrame = false;
        }
#endif

        m_ScriptSystem->OnUpdate();
    }

}