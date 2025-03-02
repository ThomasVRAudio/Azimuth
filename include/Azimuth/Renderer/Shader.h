#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{

    struct Uniform;

    class Shader
    {
    public:
        unsigned int ID;

        Shader(const std::string &vertexPath, const std::string &fragmentPath, bool receivesLight = false, const std::string &geometryPath = "");
        Shader(const std::filesystem::path &path);

        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat3(const std::string &name, glm::mat3 &mat) const;
        void setMat4(const std::string &name, glm::mat4 &mat) const;
        void setVec2(const std::string &name, const glm::vec2 &vec) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec3(const std::string &name, const glm::vec3 &vec) const;
        void setVec4(const std::string &name, glm::vec4 &vec) const;
        void ReloadShader();
        void setUniform(Uniform &uniform);
        inline std::pair<std::string, std::string> GetPaths() { return {m_VertPath, m_FragPath}; };
        inline bool IsLit() { return m_ReceiveLight; }

    private:
        unsigned int ProcessFile(const char *path, GLenum type);
        void InsertProperties(std::string &code);
        void LoadSingleFile(const std::filesystem::path &path);
        std::pair<int, int> ProcessSingleFile(const std::filesystem::path &path);
        std::string m_VertPath;
        std::string m_FragPath;
        bool m_ReceiveLight;
        friend class Serializer;
    };

}