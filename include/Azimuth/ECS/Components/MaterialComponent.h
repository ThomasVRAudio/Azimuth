#pragma once
#include <Azimuth/ECS/Components/IComponent.h>
#include <Azimuth/API/GameComponent.h>
#include <Azimuth/Renderer/Shader.h>
#include <Azimuth/Renderer/TextureLoader.h>
#include <variant>

namespace Azimuth
{

    struct Uniform
    {
        GLenum Type;
        std::string Name;
        std::variant<int, bool, float, glm::vec3, glm::vec4, unsigned int> Value;
    };

    class MaterialComponent : public Material, public IComponent
    {
    public:
        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent &other)
            : shader(std::make_shared<Shader>(*other.shader)),
              m_Textures(other.m_Textures), m_Uniforms(std::make_shared<std::vector<Uniform>>(*other.m_Uniforms))
        {
        }
        std::shared_ptr<Shader> shader;
        void CreateMaterial();
        void CreateMaterial(std::shared_ptr<Shader> shader);
        void CreateMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<std::vector<Uniform>> uniforms);
        std::shared_ptr<std::vector<Uniform>> GetUniforms() const { return m_Uniforms; };
        void SetUniforms();
        void UpdateUniforms();
        void BindTextures();
        void AddTexture(const std::string &name, const std::string &path, unsigned int slot);
        void AddTexture(Texture &texture);
        void Use() { shader->use(); }
        void SetUniformVec3(const std::string &name, const glm::vec3 &vec) const override;

    private:
        std::vector<Texture> m_Textures;
        std::shared_ptr<std::vector<Uniform>> m_Uniforms = std::make_shared<std::vector<Uniform>>();
        friend class Serializer;
    };
}