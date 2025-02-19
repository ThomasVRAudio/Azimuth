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
        ImGui::SeparatorText("HDR Settings");

        if (EditorManager::GetSceneSettings() != nullptr)
        {
            float &Exposure = EditorManager::GetSceneSettings()->Exposure;

            ImGui::Text("Exposure");
            ImGui::SliderFloat("##HDRExposure", &Exposure, 0.0f, max);
            Exposure = round(Exposure / 0.25f) * 0.25f;

            ImGui::Text("Skybox Intensity");
            float &HDRCubemapIntensity = EditorManager::GetSceneSettings()->HDRCubemapIntensity;
            ImGui::SliderFloat("##SkyboxIntensity", &HDRCubemapIntensity, 0.0f, 2.0f);

            ImGui::SeparatorText("Bloom Settings");

            ImGui::Text("Threshold");
            float &BloomThreshold = EditorManager::GetSceneSettings()->BloomThreshold;
            ImGui::SliderFloat("##BloomThreshold", &BloomThreshold, 0.0f, 10.0f);

            ImGui::Text("Blend Mix");
            float &BloomMix = EditorManager::GetSceneSettings()->BloomBlend;
            ImGui::SliderFloat("##BloomBlendMix", &BloomMix, 0.0f, 1.0f);
        }

        ImGui::SeparatorText("Application Settings: ");

        bool &VSync = EditorManager::GetSceneSettings()->VSync;

        ImGui::Checkbox("VSync", &VSync);

        if (VSync != m_VSyncLastCheckboxState)
        {
            if (VSync)
                Window::SetVSync(true);
            else
                Window::SetVSync(false);

            m_VSyncLastCheckboxState = VSync;
        }

        ImGui::Text("Framerate");
        float delta = 1.0f / Time::deltaTime();
        ImGui::Text("%.2f", delta);

        ImGui::SeparatorText("Project Settings: ");
        char pathBuffer[512];
        ImGui::Text("Project Path: ");
        strcpy(pathBuffer, Application::projectSettings.get()->ProjectFolder.string().c_str());
        pathBuffer[sizeof(pathBuffer) - 1] = '\0';
        ImGui::InputText("##projectpath", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoUndoRedo);

        ImGui::SameLine();
        if (ImGui::Button("Change"))
        {
            std::string filepath;
            if (Files::OpenFolderDialog(filepath))
            {
                Application::projectSettings.get()->ProjectFolder = std::filesystem::path(filepath);
                EditorFileTrayPanel::Init();
            }
        }

        ImGui::Text("Main Scene: ");
        char mainSceneBuffer[512];
        strcpy(mainSceneBuffer, Application::projectSettings.get()->MainScenePath.filename().string().c_str());
        mainSceneBuffer[sizeof(mainSceneBuffer) - 1] = '\0';
        ImGui::InputText("##mainscene", mainSceneBuffer, sizeof(mainSceneBuffer), ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_NoUndoRedo);

        ImGui::Unindent(left_padding);
        ImGui::End();
    }
}

#endif