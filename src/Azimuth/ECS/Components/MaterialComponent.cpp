#include <Azimuth/ECS/Components/MaterialComponent.h>

namespace Azimuth
{
    void MaterialComponent::CreateMaterial(std::shared_ptr<Shader> shader)
    {
        if (shader)
        {
            this->shader = shader;
        }
        else
        {
            this->shader = std::make_shared<Shader>("assets/shaders/default/solid.vert", "assets/shaders/default/solid.frag");
        }

        SetUniforms();
    }

    void MaterialComponent::SetUniforms()
    {
        GLint numActiveUniforms = 0;
        glGetProgramiv(this->shader->ID, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

        for (GLint i = 0; i < numActiveUniforms; ++i)
        {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;

            glGetActiveUniform(this->shader->ID, i, sizeof(name), &length, &size, &type, name);

            Uniform uniform;
            uniform.Name = name;
            uniform.Type = type;
            print(type);

            if (type == GL_FLOAT)
            {
                uniform.Value = 0.0f;
            }
            else if (type == GL_INT)
            {
                uniform.Value = 0;
            }
            else if (type == GL_BOOL)
            {
                uniform.Value = false;
            }
            else if (type == GL_FLOAT_VEC3)
            {
                uniform.Value = glm::vec3(0.0f, 0.0f, 0.0f);
            }
            else if (type == GL_FLOAT_VEC4)
            {
                uniform.Value = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            }
            else
            {
                print("Material return: Uniform currently not available.");
                continue;
            }
            m_Uniforms->emplace_back(uniform);
        }
    }
}