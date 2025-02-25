#ifdef AZIMUTH_EDITOR
#include <Azimuth/Editor/EditorFilepicker.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/System/Files.h>
#include <Azimuth/Editor/EditorManager.h>
#include <Azimuth/Renderer/Model.h>

namespace Azimuth
{

    const std::string EditorFilepicker::SelectFile(const char *path, std::string panelName, const std::vector<std::string> &fileExtensions, bool dockable, std::function<void()> selectionMenuCallback)
    {
        std::string filePath = "";

        ImGuiWindowFlags flags = 0;
        if (!dockable)
            flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

        ImGui::Begin(panelName.c_str(), &m_IsOpen, flags);
        ImGui::SetWindowPos(ImVec2(Window::GetWidth() / 3.0f, Window::GetHeight() / 3.0f), ImGuiCond_Once);
        ImGui::SetWindowSize(ImVec2(300, 400), ImGuiCond_Once);

        float left_padding = 10.0f;
        ImGui::Dummy(ImVec2(4.0f, 4.0f));
        ImGui::Indent(left_padding);

        if (selectionMenuCallback != nullptr)
            selectionMenuCallback();

        std::vector<std::filesystem::path> filenames = Files::GetFilesWithExtension(path, fileExtensions);

        ImGui::SeparatorText("Assets:");

        for (size_t i = 0; i < filenames.size(); ++i)
        {
            std::string filename = filenames[i].filename().stem().string();
            if (ImGui::Selectable((filename + "##" + std::to_string(i)).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
            {
                m_IsOpen = false;
                filePath = filenames[i].string();
            }
        }

        ImGui::Unindent(left_padding);
        ImGui::End();

        return filePath;
    }

}

#endif