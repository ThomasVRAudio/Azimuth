#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorSettingsPanel.h>

namespace Azimuth
{
    void EditorSettingsPanel::DrawPanel()
    {
        float left_padding = 10.0f;
        ImGui::Begin("Settings");
        ImGui::Indent(left_padding);
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Checkbox("Play Scene", &Application::s_PlayingEditorScene);

        float max = 20.0f;
        ImGui::Text("HDR Settings: ");

        if (EditorUI::GetSceneSettings() != nullptr)
        {
            float &Exposure = EditorUI::GetSceneSettings()->Exposure;

            ImGui::SliderFloat("Exposure", &Exposure, 0.0f, max);
            Exposure = round(Exposure / 0.25f) * 0.25f;

            float &HDRCubemapIntensity = EditorUI::GetSceneSettings()->HDRCubemapIntensity;
            ImGui::SliderFloat("Skybox Intensity", &HDRCubemapIntensity, 0.0f, 2.0f);
        }

        ImGui::Text("Application Settings: ");

        ImGui::Checkbox("VSync", &m_VSyncCheckboxState);

        if (m_VSyncCheckboxState != m_VSyncLastCheckboxState)
        {
            if (m_VSyncCheckboxState)
                Window::SetVSync(true);
            else
                Window::SetVSync(false);

            m_VSyncLastCheckboxState = m_VSyncCheckboxState;
        }

        ImGui::Text("Framerate");
        float delta = 1.0f / Time::deltaTime();
        ImGui::Text("%.2f", delta);

        ImGui::Unindent(left_padding);
        ImGui::End();
    }
}

#endif