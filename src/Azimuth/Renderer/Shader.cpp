#include <Azimuth/Renderer/Shader.h>

namespace Azimuth
{

    unsigned int Shader::ProcessFile(const char *path, GLenum type)
    {
        std::string code;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(std::filesystem::current_path() / ".." / path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();

            code = stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
        }

        unsigned int shader;
        int success;
        char infoLog[512];

        shader = glCreateShader(type);
        const char *shaderCode = code.c_str();
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR:: " << type << "::COMPILATION::FAILED\n"
                      << infoLog << std::endl;
        }

        return shader;
    }

    Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
    {
        unsigned int vertex = ProcessFile(vertexPath, GL_VERTEX_SHADER);
        unsigned int fragment = ProcessFile(fragmentPath, GL_FRAGMENT_SHADER);
        unsigned int geometry = 0;

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        if (geometryPath)
        {
            geometry = ProcessFile(geometryPath, GL_GEOMETRY_SHADER);
            glAttachShader(ID, geometry);
        }

        glLinkProgram(ID);

        int success;
        char infoLog[512];

        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometry)
            glDeleteShader(geometry);
    };

    void Shader::use()
    {
        glUseProgram(ID);
    }

    void Shader::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setMat3(const std::string &name, glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4(const std::string &name, glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void Shader::setVec3(const std::string &name, glm::vec3 &vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void Shader::setVec4(const std::string &name, glm::vec4 &vec) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }
}