#include <Azimuth/Renderer/Shader.h>
#include <Azimuth/ECS/Components/MaterialComponent.h>

namespace Azimuth
{

    unsigned int Shader::ProcessFile(const char *path, GLenum type)
    {
        std::string code;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(std::filesystem::current_path() / path);

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

    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, bool receivesLight, const std::string &geometryPath)
        : m_VertPath(vertexPath), m_FragPath(fragmentPath), m_ReceiveLight(receivesLight)
    {
        unsigned int vertex = ProcessFile(m_VertPath.c_str(), GL_VERTEX_SHADER);
        unsigned int fragment = ProcessFile(m_FragPath.c_str(), GL_FRAGMENT_SHADER);
        unsigned int geometry = 0;

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        if (geometryPath.length() != 0)
        {
            geometry = ProcessFile(geometryPath.c_str(), GL_GEOMETRY_SHADER);
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

    Shader::Shader(const std::filesystem::path &path)
        : m_VertPath(path.string()), m_FragPath(path.string())
    {
        LoadSingleFile(path);
    }

    void Shader::LoadSingleFile(const std::filesystem::path &path)
    {
        auto [vertex, fragment] = ProcessSingleFile(path);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

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
    }

    std::pair<int, int> Shader::ProcessSingleFile(const std::filesystem::path &path)
    {
        std::string code;
        std::ifstream file;

        std::string vertexCode;
        std::string fragmentCode;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();

            code = stream.str();
            InsertProperties(code);

            std::string vertexMarker = "===== VERTEX SHADER =====";
            std::string fragmentMarker = "===== FRAGMENT SHADER =====";

            size_t vertexPos = code.find(vertexMarker);
            size_t fragmentPos = code.find(fragmentMarker);

            if (vertexPos == std::string::npos || fragmentPos == std::string::npos)
            {
                std::cerr << "ERROR::SHADER::MARKERS_NOT_FOUND in file: " << path << std::endl;
                return {-1, -1};
            }

            size_t vertexLineEnd = code.find('\n', vertexPos);
            if (vertexLineEnd != std::string::npos)
                code.replace(vertexPos, vertexLineEnd - vertexPos, "#version 460 core\n");

            size_t fragmentLineEnd = code.find('\n', fragmentPos);
            if (fragmentLineEnd != std::string::npos)
                code.replace(fragmentPos, fragmentLineEnd - fragmentPos, "#version 460 core\n");

            vertexCode = code.substr(vertexPos, code.rfind('\n', fragmentPos) - vertexPos);
            fragmentCode = code.substr(fragmentPos);

            if (fragmentCode.find("#define AZIMUTH_LIT_PROPERTIES") != std::string::npos)
                InsertLitProperties(fragmentCode);
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
        }

        unsigned int vertOutput, fragOutput;
        int success;
        char infoLog[512];

        vertOutput = glCreateShader(GL_VERTEX_SHADER);
        const char *vert = vertexCode.c_str();
        glShaderSource(vertOutput, 1, &vert, NULL);
        glCompileShader(vertOutput);

        glGetShaderiv(vertOutput, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertOutput, 512, NULL, infoLog);
            std::cout << "ERROR::VERTEX SHADER SINGLE FILE::COMPILATION::FAILED\n"
                      << infoLog << std::endl;
        }

        fragOutput = glCreateShader(GL_FRAGMENT_SHADER);
        const char *frag = fragmentCode.c_str();
        glShaderSource(fragOutput, 1, &frag, NULL);
        glCompileShader(fragOutput);

        glGetShaderiv(fragOutput, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragOutput, 512, NULL, infoLog);
            std::cout << "ERROR::VERTEX SHADER SINGLE FILE::COMPILATION::FAILED\n"
                      << infoLog << std::endl;
        }

        return {vertOutput, fragOutput};
    }

    void Shader::InsertProperties(std::string &code)
    {
        std::unordered_map<std::string, std::string> replacements = {
            {"#define AZIMUTH_MVP_UNIFORMS", "uniform mat4 g_Model;\n"
                                             "uniform mat4 g_View;\n"
                                             "uniform mat4 g_Projection;\n"},
            {"AZIMUTH_FRAG", "vec3(g_Model * vec4(aPos, 1.0))"},
            {"AZIMUTH_NORMAL", "mat3(transpose(inverse(g_Model))) * aNormal"},
            {"AZIMUTH_POSITION", "g_Projection * g_View * g_Model * vec4(aPos, 1.0)"}};

        for (const auto &[placeholder, replacement] : replacements)
        {
            size_t pos;
            while ((pos = code.find(placeholder)) != std::string::npos)
                code.replace(pos, placeholder.length(), replacement);
        }
    }

    void Shader::InsertLitProperties(std::string &code)
    {
        std::unordered_map<std::string, std::string> replacements = {
            {"#define AZIMUTH_LIT_PROPERTIES", "#define MAX_POINT_LIGHTS 10\n"
                                               "struct DirLight {\n"
                                               "    vec3 direction;\n"
                                               "    vec3 ambient;\n"
                                               "    vec3 diffuse;\n"
                                               "    vec3 specular;\n"
                                               "};\n\n"
                                               "struct PointLight {\n"
                                               "    vec3 position;\n"
                                               "    float constant;\n"
                                               "    float linear;\n"
                                               "    float quadratic;\n"
                                               "    vec3 ambient;\n"
                                               "    vec3 diffuse;\n"
                                               "    vec3 specular;\n"
                                               "};\n\n"
                                               "uniform vec3 g_ViewPos;\n"
                                               "uniform DirLight g_DirLight;\n"
                                               "uniform int g_NumPointLights;\n"
                                               "uniform PointLight g_PointLights[MAX_POINT_LIGHTS];\n"},
            {"AZIMUTH_DIR_LIGHT", "g_DirLight"},
            {"AZIMUTH_VIEW_POS", "g_ViewPos"},
            {"AZIMUTH_NUM_POINT_LIGHTS", "g_NumPointLights"},
            {"AZIMUTH_POINT_LIGHT", "PointLight"},
            {"AZIMUTH_POINT_LIGHTS", "g_PointLights"}};

        for (const auto &[placeholder, replacement] : replacements)
        {
            size_t pos;
            while ((pos = code.find(placeholder)) != std::string::npos)
                code.replace(pos, placeholder.length(), replacement);
        }
    }

    void Shader::ReloadShader()
    {
        LoadSingleFile(std::filesystem::path(m_VertPath));
    }

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

    void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void Shader::setVec4(const std::string &name, glm::vec4 &vec) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    void Shader::setUniform(Uniform &uniform)
    {
        if (uniform.Name.find("g_") != std::string::npos)
            return;

        switch (uniform.Type)
        {
        case GL_FLOAT:
        {
            float &value = std::get<float>(uniform.Value);
            setFloat(uniform.Name, value);
            break;
        }
        case GL_INT:
        {
            int &value = std::get<int>(uniform.Value);
            setInt(uniform.Name, value);
            break;
        }
        case GL_UNSIGNED_INT:
        {
            int &value = std::get<int>(uniform.Value);
            setInt(uniform.Name, value);
            break;
        }
        case GL_BOOL:
        {
            bool &value = std::get<bool>(uniform.Value);
            setBool(uniform.Name, value);
            break;
        }
        case GL_FLOAT_VEC3:
        {
            glm::vec3 &value = std::get<glm::vec3>(uniform.Value);
            setVec3(uniform.Name, value);
            break;
        }
        case GL_FLOAT_VEC4:
        {
            glm::vec4 &value = std::get<glm::vec4>(uniform.Value);
            setVec4(uniform.Name, value);
        }
        }
    }
}