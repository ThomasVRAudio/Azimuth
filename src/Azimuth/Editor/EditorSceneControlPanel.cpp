#include <Azimuth/Editor/EditorSceneControlPanel.h>
#include <Azimuth/Editor/EditorTextureLoader.h>
#include <Azimuth/Scripts/ScriptModuleLoader.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Common.h>

namespace Azimuth
{
    void EditorSceneControlPanel::DrawPanel()
    {
        ImVec2 image_padding(2.0f, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, image_padding);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::SetNextWindowPos(ImVec2(0, 50.0f), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGui::Begin("##SceneControls", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoScrollWithMouse |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBackground);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        float availableHeight = ImGui::GetContentRegionAvail().y - image_padding.y * 4.0f;
        ImVec2 buttonSize = availableHeight > 0.1f ? ImVec2(availableHeight, availableHeight) : ImVec2(0.1f, 0.1f);

        ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (((buttonSize.x - image_padding.x) * 3.0f) * 0.5f));

        ImVec2 backgroundPos = ImGui::GetCursorPos();
        backgroundPos.y += 50.0f;
        backgroundPos.x -= image_padding.x * 2.0f;
        ImVec2 backgroundSize = ImVec2((buttonSize.x + image_padding.x * 6.0f) * 3.0f, buttonSize.y + image_padding.y);
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec4 backgroundColor(0.0f, 0.0f, 0.0f, 0.6f);

        drawList->AddRectFilled(backgroundPos, ImVec2(backgroundPos.x + backgroundSize.x, backgroundPos.y + backgroundSize.y),
                                IM_COL32(backgroundColor.x * 255, backgroundColor.y * 255, backgroundColor.z * 255, backgroundColor.w * 255),
                                4.0f, ImDrawFlags_RoundCornersAll);

        ImGui::BeginDisabled(m_PlayState == PlayState::PLAYING);
        if (ImGui::ImageButton("play_button", EditorTextureLoader::GetTextureID("play"), buttonSize - image_padding))
        {
            ScriptModuleLoader::LoadModule();
            Application::s_PlayingEditorScene = true;
            m_PlayState = PlayState::PLAYING;
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::BeginDisabled(m_PlayState == PlayState::STOPPED);
        if (ImGui::ImageButton("stop_button", EditorTextureLoader::GetTextureID("stop"), buttonSize - image_padding))
        {
            Application::s_PlayingEditorScene = false;
            ScriptModuleLoader::UnloadModule();
            m_PlayState = PlayState::STOPPED;
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::BeginDisabled(m_PlayState != PlayState::PLAYING);
        if (ImGui::ImageButton("pause_button", EditorTextureLoader::GetTextureID("pause"), buttonSize - image_padding))
        {
            Application::s_PlayingEditorScene = false;
            m_PlayState = PlayState::PAUSED;
        }
        ImGui::EndDisabled();

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(2);

        ImGui::End();
    }
}