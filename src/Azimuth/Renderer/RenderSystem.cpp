#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Renderer/LightSystem.h>

namespace Azimuth
{

    void RenderSystem::Init(ECSManager *ECS, std::shared_ptr<LightSystem> lightSystem)
    {
        this->m_ECS = ECS;
        m_LightSystem = lightSystem;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);

        HDRCubemap::LoadHDRCubemap("assets/hdr/CasualDay4K.hdr", 4096);
    }

    void RenderSystem::DrawScene(Camera &camera)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = camera.GetProjectionMatrix();

        HDRCubemap::DrawHDRCubemap(viewMatrix, projectionMatrix);

        for (auto &entity : m_Entities)
        {
            MeshComponent &mesh = m_ECS->GetComponent<MeshComponent>(entity);
            MaterialComponent &material = m_ECS->GetComponent<MaterialComponent>(entity);
            m_Model = glm::mat4(1.0f);

            TransformComponent &transform = m_ECS->GetComponent<TransformComponent>(entity);
            m_Model = glm::translate(m_Model, transform.Position);
            m_Model = glm::rotate(m_Model, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            m_Model = glm::rotate(m_Model, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            m_Model = glm::rotate(m_Model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            m_Model = glm::scale(m_Model, m_ECS->GetComponent<TransformComponent>(entity).Scale);

            material.shader->use();
            material.shader->setMat4("g_Model", m_Model);
            material.shader->setMat4("g_View", viewMatrix);
            material.shader->setMat4("g_Projection", projectionMatrix);

            std::shared_ptr<Light> directionalLight = m_LightSystem->DirectionalLight;
            if (directionalLight)
            {
                glm::mat4 lightTransform = glm::mat4(1.0f);
                lightTransform = glm::rotate(lightTransform, glm::radians(directionalLight->Transform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                lightTransform = glm::rotate(lightTransform, glm::radians(directionalLight->Transform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                lightTransform = glm::rotate(lightTransform, glm::radians(directionalLight->Transform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

                glm::vec3 forward = glm::normalize(glm::vec3(lightTransform * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));

                material.shader->setVec3("g_DirLight.direction", forward);
                material.shader->setVec3("g_DirLight.ambient", *directionalLight->Color);
                material.shader->setVec3("g_DirLight.diffuse", *directionalLight->Color);
                material.shader->setVec3("g_DirLight.specular", glm::vec3(1.0f));
            }

            material.shader->setVec3("g_ViewPos", camera.Position);
            material.shader->setInt("g_NumPointLights", m_LightSystem->PointLights.size());

            for (size_t i = 0; i < m_LightSystem->PointLights.size(); ++i)
            {
                auto &light = m_LightSystem->PointLights[i];

                material.shader->setFloat("g_PointLights[" + std::to_string(i) + "].constant", 1.0f);
                material.shader->setFloat("g_PointLights[" + std::to_string(i) + "].linear", 0.009f);
                material.shader->setFloat("g_PointLights[" + std::to_string(i) + "].quadratic", 0.0032f);
                material.shader->setVec3("g_PointLights[" + std::to_string(i) + "].position", light->Transform->Position);
                material.shader->setVec3("g_PointLights[" + std::to_string(i) + "].ambient", *light->Color);
                material.shader->setVec3("g_PointLights[" + std::to_string(i) + "].diffuse", *light->Color);
                material.shader->setVec3("g_PointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f));
            }

            for (auto &uniform : *material.GetUniforms())
                material.shader->setUniform(uniform);

            material.Use();
            mesh.DrawMesh();
        }
    };
}