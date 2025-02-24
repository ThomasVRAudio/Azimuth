#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/ECS/ECSManager.h>
#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/Core/Time.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Events/MouseEvents.h>

namespace Azimuth
{

    void EditorLayer::Init(Scene *scene)
    {
        ECSManager *ECS = scene->GetECSManager();
        ComponentMask mask;

        m_SceneSettings = scene->Settings;

        mask.set(ECS->GetComponentBitType<TransformComponent>(), true);
        mask.set(ECS->GetComponentBitType<MeshComponent>(), true);
        m_RenderSystem = ECS->RegisterSystem<RenderSystem>();
        ECS->SetSystemComponentMask<RenderSystem>(mask);

        mask.reset();
        mask.set(ECS->GetComponentBitType<LightComponent>(), true);
        mask.set(ECS->GetComponentBitType<TransformComponent>(), true);
        m_LightSystem = ECS->RegisterSystem<LightSystem>();

        ECS->SetSystemComponentMask<LightSystem>(mask);

        m_LightSystem->Init(ECS);
        m_RenderSystem->Init(ECS, m_LightSystem);

        EditorManager::Init(scene);
        EditorManager::SetLightsUpdateCallback([&]()
                                               { m_LightSystem->UpdateLights(); });

        unsigned int width = static_cast<unsigned int>(m_EditorSceneTextureWidth);
        unsigned int height = static_cast<unsigned int>(m_EditorSceneTextureWidth * (9.0f / 16.0f));

        m_FrameBufferConfig = std::make_unique<FrameBufferConfig>(ColorAttachment{width, height, &m_EditorSceneTexture});
        FrameBuffer::CreateFramebuffer(m_FrameBufferConfig.get());

        ColorAttachment entityIDAttachment{
            width,
            height,
            &m_EditorIDTexture,
            FrameBufferTextureFormat::RED_INTEGER,
            FrameBufferTextureFormat::RED_INTEGER_INTERNAL};
        DepthAttachment depth{width, height};

        m_FrameBufferEditorConfig = std::make_unique<FrameBufferConfig>(entityIDAttachment, depth);
        FrameBuffer::CreateFramebuffer(m_FrameBufferEditorConfig.get());

        m_EditorShader = std::make_shared<Shader>("assets/shaders/editor/selection/unlit.vert", "assets/shaders/editor/selection/unlit.frag");
    }

    void EditorLayer::OnStart() {}

    void EditorLayer::OnUpdate()
    {
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);

        m_RenderSystem->RenderScene(m_EditorCamera, m_FrameBufferConfig->ID, m_SceneSettings.get());
        m_RenderSystem->RenderEditorPass(m_EditorCamera, m_FrameBufferEditorConfig->ID, m_EditorShader.get(), m_EditorIDTexture);

        EditorManager::OnUpdate(m_EditorCamera, m_FrameBufferConfig.get(), m_FrameBufferEditorConfig.get());

        m_EditorCamera.ProcessKeyboard();
    }
}

#endif