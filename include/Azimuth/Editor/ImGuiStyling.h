#pragma once
#ifdef AZIMUTH_EDITOR
#include "Azimuth/Common.h"

namespace Azimuth
{
    class ImGuiStyling
    {
    public:
        static void SetStyling()
        {
            ImVec4 *colors = ImGui::GetStyle().Colors;

            ImVec4 BACKGROUND = ImVec4(0.1014f, 0.1014f, 0.1014f, 1.00f);      // rgb(59, 59, 59)
            ImVec4 BACKGROUND_DARK = ImVec4(0.2294f, 0.2294f, 0.2294f, 1.00f); // rgb(84, 84, 84)
            ImVec4 BORDER = ImVec4(0.4588f, 0.4588f, 0.4588f, 1.00f);          // rgb(117, 117, 117)
            ImVec4 MID = ImVec4(0.3275f, 0.3275f, 0.3275f, 1.00f);             // rgb(160, 160, 160)
            ImVec4 HIGHLIGHT = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);                // rgb(204, 204, 204)
            ImVec4 TEXT_PRIMARY = ImVec4(0.8980f, 0.8980f, 0.8980f, 1.00f);    // rgb(229, 229, 229)
            ImVec4 TEXT_SECONDARY = ImVec4(0.9451f, 0.9451f, 0.9451f, 1.00f);  // rgb(241, 241, 241)
            ImVec4 TEXT_DISABLED = ImVec4(0.6275f, 0.6275f, 0.6275f, 1.00f);   // rgb(160, 160, 160)

            colors[ImGuiCol_WindowBg] = BACKGROUND;
            colors[ImGuiCol_ChildBg] = BACKGROUND;
            colors[ImGuiCol_Border] = BORDER;
            colors[ImGuiCol_BorderShadow] = BORDER;
            colors[ImGuiCol_FrameBg] = BORDER;
            colors[ImGuiCol_FrameBgHovered] = MID;
            colors[ImGuiCol_FrameBgActive] = MID;
            colors[ImGuiCol_TitleBg] = BACKGROUND_DARK;
            colors[ImGuiCol_TitleBgCollapsed] = BACKGROUND_DARK;
            colors[ImGuiCol_TitleBgActive] = BACKGROUND_DARK;
            colors[ImGuiCol_MenuBarBg] = BACKGROUND;
            colors[ImGuiCol_ScrollbarBg] = BACKGROUND;
            colors[ImGuiCol_ScrollbarGrab] = MID;
            colors[ImGuiCol_ScrollbarGrabHovered] = BORDER;
            colors[ImGuiCol_Button] = MID;
            colors[ImGuiCol_ButtonHovered] = HIGHLIGHT;
            colors[ImGuiCol_ButtonActive] = MID;
            colors[ImGuiCol_Header] = MID;
            colors[ImGuiCol_HeaderHovered] = HIGHLIGHT;
            colors[ImGuiCol_HeaderActive] = BORDER;
            colors[ImGuiCol_Separator] = BORDER;
            colors[ImGuiCol_SeparatorHovered] = MID;
            colors[ImGuiCol_SeparatorActive] = BORDER;
            colors[ImGuiCol_CheckMark] = HIGHLIGHT;
            colors[ImGuiCol_SliderGrab] = MID;
            colors[ImGuiCol_SliderGrabActive] = BORDER;
            colors[ImGuiCol_Text] = TEXT_PRIMARY;
            colors[ImGuiCol_TextDisabled] = TEXT_DISABLED;
            colors[ImGuiCol_PopupBg] = BACKGROUND;
            colors[ImGuiCol_ResizeGrip] = MID;
            colors[ImGuiCol_ResizeGripHovered] = HIGHLIGHT;
            colors[ImGuiCol_ResizeGripActive] = BORDER;
            colors[ImGuiCol_Tab] = MID;
            colors[ImGuiCol_TabHovered] = HIGHLIGHT;
            colors[ImGuiCol_TabActive] = BORDER;
            colors[ImGuiCol_TabUnfocused] = BACKGROUND_DARK;
            colors[ImGuiCol_TabUnfocusedActive] = BORDER;
            colors[ImGuiCol_DockingPreview] = HIGHLIGHT;
            colors[ImGuiCol_DockingEmptyBg] = BACKGROUND;
            colors[ImGuiCol_TableHeaderBg] = BACKGROUND_DARK;
            colors[ImGuiCol_TableBorderStrong] = BORDER;
            colors[ImGuiCol_TableBorderLight] = MID;
            colors[ImGuiCol_TableRowBg] = BACKGROUND;
            colors[ImGuiCol_TableRowBgAlt] = BACKGROUND_DARK;
            colors[ImGuiCol_NavHighlight] = HIGHLIGHT;
            colors[ImGuiCol_NavWindowingHighlight] = HIGHLIGHT;
            colors[ImGuiCol_NavWindowingDimBg] = BACKGROUND_DARK;
            colors[ImGuiCol_ModalWindowDimBg] = BACKGROUND_DARK;
            colors[ImGuiCol_TabSelectedOverline] = BACKGROUND;

            ImGui::GetStyle().FrameRounding = 2;
            ImGui::GetStyle().FramePadding = ImVec2(7, 3);
            ImGui::GetStyle().ItemSpacing = ImVec2(10, 7);
            ImGui::GetStyle().ScrollbarSize = 10;
            ImGui::GetStyle().WindowBorderSize = 0;
            ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
            ImGui::GetStyle().ScrollbarRounding = 0;
            ImGui::GetStyle().GrabRounding = 2;
            ImGui::GetStyle().CellPadding = ImVec2(2, 2);
            ImGui::GetStyle().WindowTitleAlign = ImVec2(0.50f, 0.50f);
            ImGui::GetStyle().DockingSeparatorSize = 1;
            ImGui::GetStyle().SeparatorTextAlign = ImVec2(0.10f, 0.0f);
        }
    };
}

#endif