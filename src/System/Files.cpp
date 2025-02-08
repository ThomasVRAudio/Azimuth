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
}