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
        }
        ImGui::Unindent(left_padding);
        ImGui::End();
    }
}

#endif