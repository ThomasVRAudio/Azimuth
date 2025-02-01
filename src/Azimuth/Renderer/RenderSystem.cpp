#include <Azimuth/Renderer/RenderSystem.h>

namespace Azimuth
{

    void RenderSystem::Init(ECSManager *ECS, std::shared_ptr<LightSystem> lightSystem, unsigned int width, unsigned int height)
    {

        this->m_ECS = ECS;
        m_LightSystem = lightSystem;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);

        HDRCubemap::LoadHDRCubemap("assets/hdr/CasualDay4K.hdr", 4096);

        ColorAttachment color = {width, height, &m_SceneTexture};
        DepthAttachment depth = {width, height};

        m_SceneFrameBuffer = std::make_unique<FrameBufferConfig>(color, depth);
        FrameBuffer::CreateFramebuffer(m_SceneFrameBuffer.get());

        m_PostProcessingShader = std::make_unique<Shader>("assets/shaders/default/postprocessing.vert", "assets/shaders/default/postprocessing.frag");
    }

    void RenderSystem::RenderScene(Camera &camera, unsigned int outputFramebuffer)
    {
        FrameBuffer::BindFramebuffer(&m_SceneFrameBuffer->ID);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        RenderPass(camera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        FrameBuffer::BindFramebuffer(&outputFramebuffer);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        RenderScreenQuad(m_PostProcessingShader.get(), m_SceneTexture);

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
    void RenderSystem::RenderEditorPass(Camera &camera, unsigned int framebuffer, Shader *shader, unsigned int texture)
    {
        FrameBuffer::BindFramebuffer(&framebuffer);
        int clearValue = -1;
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearTexImage(texture, 0, GL_RED_INTEGER, GL_INT, &clearValue);

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
            shader->setInt("g_Entity", entity);

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

    void RenderSystem::RenderScreenQuad(Shader *shader, unsigned int texture)
    {
        shader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        m_PostProcessingShader->setInt("g_Texture", 0);

        glBindVertexArray(m_RenderScreenQuad.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderScreenQuad.EBO);
        glDrawElements(GL_TRIANGLES, m_RenderScreenQuad.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}