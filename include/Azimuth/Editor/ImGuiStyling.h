#include "Azimuth/Common.h"

namespace Azimuth
{
    class ImGuiStyling
    {
    public:
        static void SetStyling()
        {
            ImVec4 *colors = ImGui::GetStyle().Colors;
            ImVec4 gray = ImVec4(0.37f, 0.38f, 0.39f, 1.00f); // Unified gray tone

            colors[ImGuiCol_FrameBg] = gray;
            colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.19f, 0.19f, 0.40f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.67f);
            colors[ImGuiCol_CheckMark] = gray;
            colors[ImGuiCol_SliderGrab] = gray;
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
            colors[ImGuiCol_Button] = gray;
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.41f, 0.43f, 1.00f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
            colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 0.31f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.16f, 0.16f, 0.80f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Lighter gray for active header
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.19f, 0.19f, 1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(0.19f, 0.20f, 0.20f, 0.20f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 0.67f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.07f, 0.07f, 0.07f, 0.95f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);
            colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 0.86f);
            colors[ImGuiCol_TabSelected] = ImVec4(0.39f, 0.39f, 0.32f, 1.00f);
            colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_TabDimmed] = ImVec4(0.28f, 0.28f, 0.28f, 0.97f);
            colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
            colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.18f, 0.18f, 0.18f, 0.00f);
            colors[ImGuiCol_DockingPreview] = ImVec4(0.28f, 0.28f, 0.28f, 0.70f);
            colors[ImGuiCol_TextLink] = ImVec4(0.18f, 0.39f, 0.35f, 1.00f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.32f, 0.32f, 0.35f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(0.31f, 0.31f, 0.30f, 0.90f);
            colors[ImGuiCol_NavCursor] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

            ImGui::GetStyle().FrameRounding = 2;
            ImGui::GetStyle().FramePadding = ImVec2(0, 3);
            ImGui::GetStyle().ItemSpacing = ImVec2(10, 7);
            ImGui::GetStyle().ScrollbarSize = 10;
            ImGui::GetStyle().WindowBorderSize = 0;
            ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
            ImGui::GetStyle().ScrollbarRounding = 0;
            ImGui::GetStyle().GrabRounding = 2;
            ImGui::GetStyle().CellPadding = ImVec2(2, 2);
            ImGui::GetStyle().WindowTitleAlign = ImVec2(0.50f, 0.50f);
            ImGui::GetStyle().DockingSeparatorSize = 1;
        }
    };
}