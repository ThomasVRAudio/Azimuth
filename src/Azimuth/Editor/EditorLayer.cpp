#include <Azimuth/Editor/EditorLayer.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Window.h>

namespace Azimuth
{

    void EditorLayer::Init()
    {
        ECSManager &ECS = ECSManager::GetInstance();

        ComponentMask mask;
        mask.set(ECS.GetComponentBitType<TransformComponent>(), true);
        mask.set(ECS.GetComponentBitType<MeshComponent>(), true);
        m_RenderSystem = ECS.RegisterSystem<RenderSystem>();
        ECS.SetSystemComponentMask<RenderSystem>(mask);

        m_RenderSystem->Init();
    }

    void EditorLayer::OnStart()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(Window::m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void EditorLayer::OnUpdate()
    {
        m_RenderSystem->DrawScene();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::ColorEdit3("clear color", (float *)&m_ClearColor);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
    }

}