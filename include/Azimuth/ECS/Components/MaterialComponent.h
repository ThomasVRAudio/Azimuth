#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/Renderer/Shader.h>
#include <variant>

namespace Azimuth
{

    struct Uniform
    {
        GLenum Type;
        std::string Name;
        std::variant<int, bool, float, glm::vec3, glm::vec4> Value;
    };

    class MaterialComponent : public IComponent
    {
    public:
        std::string type = "solid";
        glm::vec3 color;
        std::shared_ptr<Shader> shader;
        void CreateMaterial(std::shared_ptr<Shader> shader = nullptr);
        void SetUniforms();
        std::shared_ptr<std::vector<Uniform>> GetUniforms() { return m_Uniforms; };

    private:
        std::shared_ptr<std::vector<Uniform>> m_Uniforms = std::make_shared<std::vector<Uniform>>();
    };
}