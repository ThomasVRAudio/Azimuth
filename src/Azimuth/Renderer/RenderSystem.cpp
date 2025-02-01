#include <Azimuth/Renderer/RenderSystem.h>

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

    void RenderSystem::RenderScene(Camera &camera, unsigned int framebuffer)
    {
        FrameBuffer::BindFramebuffer(&framebuffer);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        RenderPass(camera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    void RenderSystem::RenderPass(Camera &camera)
    {
        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = camera.GetProjectionMatrix();

        HDRCubemap::DrawHDRCubemap(viewMatrix, projectionMatrix);

        for (auto &entity : m_Entities)
        {
            MeshComponent &mesh = m_ECS->GetComponent<MeshComponent>(entity);
            MaterialComponent &material = m_ECS->GetComponent<MaterialComponent>(entity);

            m_Model = m_ECS->GetComponent<TransformComponent>(entity).GetTransform();

            material.shader->use();
            material.shader->setMat4("g_Model", m_Model);
            material.shader->setMat4("g_View", viewMatrix);
            material.shader->setMat4("g_Projection", projectionMatrix);

            RenderLights(material.shader, camera);

            for (auto &uniform : *material.GetUniforms())
                material.shader->setUniform(uniform);

            material.Use();
            mesh.DrawMesh();
        }
    }

#ifdef AZIMUTH_EDITOR
    void RenderSystem::RenderEditorPass(Camera &camera, unsigned int framebuffer, Shader *shader)
    {
        FrameBuffer::BindFramebuffer(&framebuffer);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = camera.GetProjectionMatrix();

        for (auto &entity : m_Entities)
        {
            MeshComponent &mesh = m_ECS->GetComponent<MeshComponent>(entity);

            m_Model = m_ECS->GetComponent<TransformComponent>(entity).GetTransform();

            shader->use();
            shader->setMat4("g_Model", m_Model);
            shader->setMat4("g_View", viewMatrix);
            shader->setMat4("g_Projection", projectionMatrix);

            mesh.DrawMesh();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
#endif

    void RenderSystem::RenderLights(std::shared_ptr<Shader> shader, Camera &camera)
    {

        std::shared_ptr<Light> directionalLight = m_LightSystem->DirectionalLight;
        if (directionalLight)
        {
            glm::mat4 lightTransform = directionalLight->Transform->GetTransform();

            glm::vec3 forward = glm::normalize(glm::vec3(lightTransform * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f)));

            shader->setVec3("g_DirLight.direction", forward);
            shader->setVec3("g_DirLight.ambient", *directionalLight->Color);
            shader->setVec3("g_DirLight.diffuse", *directionalLight->Color);
            shader->setVec3("g_DirLight.specular", glm::vec3(1.0f));
        }

        shader->setVec3("g_ViewPos", camera.Position);
        shader->setInt("g_NumPointLights", m_LightSystem->PointLights.size());

        for (size_t i = 0; i < m_LightSystem->PointLights.size(); ++i)
        {
            auto &light = m_LightSystem->PointLights[i];

            shader->setFloat("g_PointLights[" + std::to_string(i) + "].constant", 1.0f);
            shader->setFloat("g_PointLights[" + std::to_string(i) + "].linear", 0.009f);
            shader->setFloat("g_PointLights[" + std::to_string(i) + "].quadratic", 0.0032f);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].position", light->Transform->Position);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].ambient", *light->Color);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].diffuse", *light->Color);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f));
        }
    }

}