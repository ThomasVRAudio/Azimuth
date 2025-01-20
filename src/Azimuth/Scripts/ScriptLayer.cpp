#include <Azimuth/Scripts/ScriptLayer.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void ScriptLayer::Init(Scene *scene)
    {
        ECSManager *ECS = scene->GetECSManager();

        ComponentMask mask;

        mask.set(ECS->GetComponentBitType<ScriptsComponent>(), true);
        m_ScriptSystem = ECS->RegisterSystem<ScriptSystem>();
        ECS->SetSystemComponentMask<ScriptSystem>(mask);
        m_ScriptSystem->Init(ECS);
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