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

        ColorAttachment color = {width, height, &m_RenderedSceneTexture, FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RGBA16F};
        DepthAttachment depth = {width, height};

        m_SceneRenderFrameBuffer = std::make_unique<FrameBufferConfig>(color, depth, true);
        FrameBuffer::CreateFramebuffer(m_SceneRenderFrameBuffer.get());

        ColorAttachment hdrColor = {width, height, &m_ToneMappedTexture};

        m_TonemappingFrameBuffer = std::make_unique<FrameBufferConfig>(hdrColor);
        FrameBuffer::CreateFramebuffer(m_TonemappingFrameBuffer.get());

        ColorAttachment postProcessingColor = {width, height, &m_PostProcessedTexture};
        m_PostProcessingFrameBuffer = std::make_unique<FrameBufferConfig>(postProcessingColor);
        FrameBuffer::CreateFramebuffer(m_PostProcessingFrameBuffer.get());

        m_PostProcessShader = std::make_unique<Shader>("assets/shaders/default/postprocessing.vert", "assets/shaders/default/postprocessing.frag");
        m_FinalCompositeShader = std::make_unique<Shader>("assets/shaders/default/master.vert", "assets/shaders/default/master.frag");
        m_ToneMappingShader = std::make_unique<Shader>("assets/shaders/default/hdr.vert", "assets/shaders/default/hdr.frag");
    }

    void RenderSystem::RenderScene(Camera &camera, unsigned int outputFramebuffer, SceneSettings *settings)
    {
        // Scene Rendering
        FrameBuffer::BindFramebuffer(&m_SceneRenderFrameBuffer->ID);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        RenderPass(camera);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // HDR Tone Mapping
        FrameBuffer::BindFramebuffer(&m_TonemappingFrameBuffer->ID);
        glClear(GL_COLOR_BUFFER_BIT);
        m_ToneMappingShader->use();
        m_ToneMappingShader->setFloat("g_Exposure", settings->Exposure);
        RenderScreenQuad(m_ToneMappingShader.get(), m_RenderedSceneTexture);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Post Processing
        FrameBuffer::BindFramebuffer(&m_PostProcessingFrameBuffer->ID);
        glClear(GL_COLOR_BUFFER_BIT);
        RenderScreenQuad(m_PostProcessShader.get(), m_ToneMappedTexture);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Final Output
        FrameBuffer::BindFramebuffer(&outputFramebuffer);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        RenderScreenQuad(m_FinalCompositeShader.get(), m_PostProcessedTexture);
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
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].ambient", *light->Color * *light->HDRMultiplier);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].diffuse", *light->Color * *light->HDRMultiplier);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f) * *light->HDRMultiplier);
        }
    }

    void RenderSystem::RenderScreenQuad(Shader *shader, unsigned int texture, bool mipmaps)
    {
        shader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader->setInt("g_Texture", 0);

        if (mipmaps)
            shader->setFloat("g_MipmapLevel", 1.0f);

        glBindVertexArray(m_RenderScreenQuad.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderScreenQuad.EBO);
        glDrawElements(GL_TRIANGLES, m_RenderScreenQuad.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}