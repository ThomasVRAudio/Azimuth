#include "Azimuth/Common.h"

namespace Azimuth
{
    class ImGuiStyling
    {
    public:
        static void SetStyling()
        {
            ImVec4 *colors = ImGui::GetStyle().Colors;
            // ImVec4 gray = ImVec4(0.37f, 0.38f, 0.39f, 1.00f); // Unified gray tone

            // colors[ImGuiCol_FrameBg] = gray;
            // colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.19f, 0.19f, 0.40f);
            // colors[ImGuiCol_FrameBgActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.67f);
            // colors[ImGuiCol_CheckMark] = gray;
            // colors[ImGuiCol_SliderGrab] = gray;
            // colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
            // colors[ImGuiCol_Button] = gray;
            // colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.41f, 0.43f, 1.00f);
            // colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
            // colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 0.31f);
            // colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.80f);
            // colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Lighter gray for active header
            // colors[ImGuiCol_SeparatorHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
            // colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.19f, 0.19f, 1.00f);
            // colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.20f, 0.20f, 0.20f);
            // colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 0.67f);
            // colors[ImGuiCol_ResizeGripActive] = ImVec4(0.07f, 0.07f, 0.07f, 0.95f);
            // colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
            // colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 0.86f);
            // colors[ImGuiCol_TabSelected] = ImVec4(0.39f, 0.39f, 0.32f, 1.00f);
            // colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            // colors[ImGuiCol_TabDimmed] = ImVec4(0.28f, 0.28f, 0.28f, 0.97f);
            // colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
            // colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.18f, 0.18f, 0.18f, 0.00f);
            // colors[ImGuiCol_DockingPreview] = ImVec4(0.28f, 0.28f, 0.28f, 0.70f);
            // colors[ImGuiCol_TextLink] = ImVec4(0.18f, 0.39f, 0.35f, 1.00f);
            // colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.32f, 0.32f, 0.35f);
            // colors[ImGuiCol_DragDropTarget] = ImVec4(0.31f, 0.31f, 0.30f, 0.90f);
            // colors[ImGuiCol_NavCursor] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
            // colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

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

            ImGui::GetStyle().FrameRounding = 1;
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