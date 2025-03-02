#include <Azimuth/ECS/Components/MaterialComponent.h>

namespace Azimuth
{
    void MaterialComponent::CreateMaterial(std::shared_ptr<Shader> shader)
    {
        this->shader = shader;
        SetUniforms();
    }

    void MaterialComponent::CreateMaterial()
    {
        this->shader = std::make_shared<Shader>("assets/shaders/library/default/model.vert", "assets/shaders/library/default/lit/lit.frag");
        SetUniforms();
    }

    void MaterialComponent::CreateMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<std::vector<Uniform>> uniforms)
    {
        this->shader = shader;
        m_Uniforms = uniforms;
    }

    void MaterialComponent::BindTextures()
    {
        if (shader == nullptr)
            return;

        for (size_t i = 0; i < m_Textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + m_Textures[i].slot);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    void MaterialComponent::AddTexture(Texture &texture)
    {
        texture.id = TextureLoader::LoadTexture(texture.path);
        m_Textures.emplace_back(texture);
        shader->use();
        shader->setInt(texture.name, texture.slot);
    }

    void MaterialComponent::AddTexture(const std::string &name, const std::string &path, unsigned int slot)
    {
        unsigned int offset = 1;

        Texture texture;
        texture.id = TextureLoader::LoadTexture(path);
        texture.path = path;
        texture.slot = slot + offset;
        texture.name = name;

        m_Textures.emplace_back(texture);

        shader->use();
        shader->setInt(name, slot + offset);
    }

    void MaterialComponent::SetUniforms()
    {
        m_Uniforms->clear();

        GLint numActiveUniforms = 0;
        glGetProgramiv(this->shader->ID, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

        for (GLint i = 0; i < numActiveUniforms; ++i)
        {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;

            glGetActiveUniform(this->shader->ID, i, sizeof(name), &length, &size, &type, name);
            GLenum error;
            error = glGetError();
            if (error != GL_NO_ERROR)
                print("SetUniforms Error: " << error);

            Uniform uniform;
            uniform.Name = name;
            uniform.Type = type;

            if (type == GL_FLOAT)
                uniform.Value = 1.0f;
            else if (type == GL_INT)
                uniform.Value = 1;
            else if (type == GL_SAMPLER_2D)
                uniform.Value = 0;
            else if (type == GL_BOOL)
                uniform.Value = false;
            else if (type == GL_FLOAT_VEC3)
                uniform.Value = glm::vec3(1.0f, 1.0f, 1.0f);
            else if (type == GL_FLOAT_VEC4)
                uniform.Value = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            else
                continue;
            m_Uniforms->emplace_back(uniform);
        }
    }

    void MaterialComponent::UpdateUniforms()
    {
        GLint numActiveUniforms = 0;
        glGetProgramiv(this->shader->ID, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

        std::vector<Uniform> newUniforms;
        for (GLint i = 0; i < numActiveUniforms; ++i)
        {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;

            glGetActiveUniform(this->shader->ID, i, sizeof(name), &length, &size, &type, name);

            GLenum error;
            error = glGetError();
            if (error != GL_NO_ERROR)
                print("SetUniforms Error: " << error);

            bool uniformFound = false;
            for (const auto &uniform : *m_Uniforms)
            {
                if (uniform.Name == name && uniform.Type == type)
                {
                    newUniforms.emplace_back(uniform);
                    uniformFound = true;
                    break;
                }
            }

            if (uniformFound)
                continue;

            Uniform uniform;
            uniform.Name = name;
            uniform.Type = type;

            if (type == GL_FLOAT)
                uniform.Value = 1.0f;
            else if (type == GL_INT)
                uniform.Value = 1;
            else if (type == GL_SAMPLER_2D)
                uniform.Value = 0;
            else if (type == GL_BOOL)
                uniform.Value = false;
            else if (type == GL_FLOAT_VEC3)
                uniform.Value = glm::vec3(1.0f, 1.0f, 1.0f);
            else if (type == GL_FLOAT_VEC4)
                uniform.Value = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            else
                continue;
            newUniforms.emplace_back(uniform);
        }

        m_Uniforms = std::make_shared<std::vector<Uniform>>(newUniforms);
    }
}