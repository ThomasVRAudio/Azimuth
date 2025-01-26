#include <Azimuth/Project/Serializer.h>

namespace Azimuth
{

    bool Serializer::OpenFileDialog(std::string &outFilePath, FileDialogType dialogType)
    {
        OPENFILENAME ofn;
        char szFile[260];

        ofn.hwndOwner = glfwGetWin32Window(Window::GetMainWindow());

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Scene Files\0*.scene\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        std::filesystem::path projectPath = std::filesystem::current_path();
        std::filesystem::path initialDir = projectPath / "Scenes";
        if (!std::filesystem::exists(initialDir))
            std::filesystem::create_directories(initialDir);

        std::string initialDirString = std::filesystem::absolute(initialDir).string();
        ofn.lpstrInitialDir = initialDirString.c_str();

        ofn.lpstrTitle = "Open Scene File";
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (dialogType == SAVE)
            ofn.Flags |= OFN_OVERWRITEPROMPT;

        bool result = (dialogType == OPEN) ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);

        if (result)
        {
            outFilePath = szFile;
            return true;
        }

        return false;
    }

    void Serializer::OpenScene()
    {
        std::string filePath;
        if (OpenFileDialog(filePath, OPEN))
        {
            print("in open: " << filePath);
        }
    }

    void Serializer::SaveScene(Scene *scene)
    {
        std::string filePath;
        if (OpenFileDialog(filePath, SAVE))
        {
            for (auto &entity : scene->m_Entities)
            {
                if (scene->ECS->HasComponent<TransformComponent>(entity))
                {
                    // save in yaml form
                }
            }
            print("in save");
        }
    }
}