#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>
#include <Azimuth/Renderer/LightSystem.h>

namespace Azimuth
{

    void RenderSystem::Init(ECSManager *ECS, std::vector<std::shared_ptr<Light>> *lights)
    {
        this->m_ECS = ECS;
        this->m_Lights = lights;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);

        std::vector<std::string> faces = {
            "assets/skybox/ice/right.jpg",
            "assets/skybox/ice/left.jpg",
            "assets/skybox/ice/top.jpg",
            "assets/skybox/ice/bottom.jpg",
            "assets/skybox/ice/front.jpg",
            "assets/skybox/ice/back.jpg"};

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

            for (auto &light : *m_Lights)
            {
                if (*light->Type == DIRECTIONAL_LIGHT)
                {
                    glm::mat4 lightTransform = glm::mat4(1.0f);
                    lightTransform = glm::rotate(lightTransform, glm::radians(light->Transform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                    lightTransform = glm::rotate(lightTransform, glm::radians(light->Transform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                    lightTransform = glm::rotate(lightTransform, glm::radians(light->Transform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

                    glm::vec3 forward = glm::normalize(glm::vec3(lightTransform * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));

                    material.shader->setVec3("g_DirLight.direction", forward);
                    material.shader->setVec3("g_DirLight.ambient", *light->Color);
                    material.shader->setVec3("g_DirLight.diffuse", *light->Color);
                    material.shader->setVec3("g_DirLight.specular", glm::vec3(1.0f));
                    material.shader->setVec3("g_ViewPos", camera.Position);
                }
            }

            for (auto &uniform : *material.GetUniforms())
                material.shader->setUniform(uniform);

            material.Use();
            mesh.DrawMesh();
        }
    };
}