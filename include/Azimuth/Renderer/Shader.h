#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{

    class Shader
    {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);
        // use/activate the shader
        void use();
        // utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat3(const std::string &name, glm::mat3 &mat) const;
        void setMat4(const std::string &name, glm::mat4 &mat) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec3(const std::string &name, glm::vec3 &vec) const;

    private:
        unsigned int ProcessFile(const char *path, GLenum type);
    };

}