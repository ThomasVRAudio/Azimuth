#include <Azimuth/System/Files.h>

namespace Azimuth
{
    std::vector<std::pair<FileType, std::string>> Files::GetFilenamesFromDirectory(const std::string &directory)
    {
        std::vector<std::pair<FileType, std::string>> files;

        if (std::filesystem::current_path().filename() == "Scenes")
            std::filesystem::current_path(std::filesystem::current_path().parent_path());

        for (const auto &file : std::filesystem::directory_iterator(directory))
        {
            files.emplace_back(std::pair<FileType, std::string>(file.is_regular_file() ? FileType::File : FileType::Directory, file.path().filename().string()));
        }

        return files;
    }

    std::vector<std::filesystem::path> Files::GetFilesWithExtension(const std::filesystem::path &directory, const std::vector<std::string> &extensions)
    {
        std::vector<std::filesystem::path> files;

        for (const auto &file : std::filesystem::directory_iterator(directory))
        {
            if (file.is_directory())
            {

                std::vector<std::filesystem::path> childFiles = GetFilesWithExtension(file.path(), extensions);
                files.insert(files.end(), childFiles.begin(), childFiles.end());
            }
            else if (std::find(extensions.begin(), extensions.end(), file.path().extension()) != extensions.end())
            {
                files.emplace_back(file.path());
            }
        }

        return files;
    }

    bool Files::GetShaderInfoFromFile(const std::string &filepath, std::string &vertPath, bool &lit)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            return false;
        }

        std::string line;
        bool foundVert = false;
        bool foundLit = false;

        while (std::getline(file, line))
        {
            if (line.find("# Vert:") == 0)
            {
                std::smatch match;
                std::regex vertRegex(R"(^# Vert:\s*(\S+))");
                if (std::regex_search(line, match, vertRegex))
                {
                    vertPath = match[1].str();
                    foundVert = true;
                }
            }
            if (line.find("# Lit:") == 0)
            {
                lit = (line.find("1") != std::string::npos);
                foundLit = true;
            }

            if (foundVert && foundLit)
                break;
        }

        return foundVert && foundLit;
    }

    bool Files::OpenFolderDialog(std::string &outFolderPath)
    {
        CoInitialize(nullptr);

        IFileDialog *pfd;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pfd));
        if (FAILED(hr))
        {
            CoUninitialize();
            return false;
        }

        DWORD dwOptions;
        pfd->GetOptions(&dwOptions);
        pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

        std::wstring initialPath = Application::projectSettings->ProjectFolder.wstring();
        IShellItem *pInitialFolder;
        hr = SHCreateItemFromParsingName(initialPath.c_str(), nullptr, IID_PPV_ARGS(&pInitialFolder));
        if (SUCCEEDED(hr))
        {
            pfd->SetFolder(pInitialFolder);
            pInitialFolder->Release();
        }
        else
        {
            pfd->Release();
            CoUninitialize();
            return false;
        }

        hr = pfd->Show(nullptr);
        if (SUCCEEDED(hr))
        {
            IShellItem *psi;
            hr = pfd->GetResult(&psi);
            if (SUCCEEDED(hr))
            {
                PWSTR pszPath = nullptr;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                if (SUCCEEDED(hr))
                {
                    std::wstring wstr(pszPath);
                    outFolderPath = std::string(wstr.begin(), wstr.end());
                    CoTaskMemFree(pszPath);
                    psi->Release();
                    pfd->Release();
                    CoUninitialize();
                    return true;
                }
                psi->Release();
            }
        }

        pfd->Release();
        CoUninitialize();
        return false;
    }
}