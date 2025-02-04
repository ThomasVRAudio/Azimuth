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

        ColorAttachment sceneColor = {width, height, &m_RenderedSceneTexture, FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RGBA16F};
        DepthAttachment sceneDepth = {width, height};
        m_SceneRenderFrameBuffer = std::make_unique<FrameBufferConfig>(sceneColor, sceneDepth, true);
        FrameBuffer::CreateFramebuffer(m_SceneRenderFrameBuffer.get());

        ColorAttachment tonemapColor = {width, height, &m_ToneMappedTexture};
        m_TonemappingFrameBuffer = std::make_unique<FrameBufferConfig>(tonemapColor);
        FrameBuffer::CreateFramebuffer(m_TonemappingFrameBuffer.get());

        ColorAttachment postProcessColor = {width, height, &m_PostProcessedTexture};
        m_PostProcessingFrameBuffer = std::make_unique<FrameBufferConfig>(postProcessColor);
        FrameBuffer::CreateFramebuffer(m_PostProcessingFrameBuffer.get());

        m_ToneMappingShader = std::make_unique<Shader>("assets/shaders/default/tonemapping.vert", "assets/shaders/default/tonemapping.frag");
        m_PostProcessShader = std::make_unique<Shader>("assets/shaders/default/postprocessing.vert", "assets/shaders/default/postprocessing.frag");
        m_FinalCompositeShader = std::make_unique<Shader>("assets/shaders/default/master.vert", "assets/shaders/default/master.frag");

        // Bloom
        glm::vec2 currentMipSize = {width, height};

        for (size_t i = 0; i < mipChainLength; i++)
        {
            currentMipSize *= (i == 0) ? 1.0f : 0.5f;
            ColorAttachment mipColorAttachment = {static_cast<unsigned int>(currentMipSize.x), static_cast<unsigned int>(currentMipSize.y), &mipTextures[i],
                                                  FrameBufferTextureFormat::RGB, FrameBufferTextureFormat::R11FG11FB10F};

            m_BloomMipChainAttachments.emplace_back(mipColorAttachment);
        }

        m_BloomFrameBuffer = std::make_unique<FrameBufferConfig>(m_BloomMipChainAttachments);
        m_BloomFrameBuffer->singleRenderOutput = true;

        FrameBuffer::CreateFramebuffer(m_BloomFrameBuffer.get());

        m_BloomDownSampleShader = std::make_unique<Shader>("assets/shaders/default/bloom/bloom.vert", "assets/shaders/default/bloom/downsample.frag");
        m_BloomUpSampleShader = std::make_unique<Shader>("assets/shaders/default/bloom/bloom.vert", "assets/shaders/default/bloom/upsample.frag");

        m_BloomDownSampleShader->use();
        m_BloomDownSampleShader->setInt("srcTexture", 0);
        m_BloomUpSampleShader->use();
        m_BloomUpSampleShader->setInt("srcTexture", 0);
    }

    void RenderSystem::RenderScene(Camera &camera, unsigned int outputFramebuffer, SceneSettings *settings)
    {
        // Scene Rendering
        FrameBuffer::BindFramebuffer(&m_SceneRenderFrameBuffer->ID);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        RenderPass(camera, settings);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Bloom
        FrameBuffer::BindFramebuffer(&m_BloomFrameBuffer->ID);
        glClear(GL_COLOR_BUFFER_BIT);
        BloomDownSampling(m_RenderedSceneTexture);
        BloomUpSampling(0.0005f);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *m_BloomFrameBuffer->colorAttachments[0].texture);

        // HDR Tone Mapping
        FrameBuffer::BindFramebuffer(&m_TonemappingFrameBuffer->ID);
        glClear(GL_COLOR_BUFFER_BIT);
        m_ToneMappingShader->use();
        m_ToneMappingShader->setFloat("g_Exposure", settings->Exposure);
        RenderScreenQuad(m_ToneMappingShader.get(), *m_BloomFrameBuffer->colorAttachments[0].texture);
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

    void RenderSystem::RenderPass(Camera &camera, SceneSettings *settings)
    {
        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = camera.GetProjectionMatrix();

        float intensity = 1.0f;
        if (settings)
            intensity = settings->HDRCubemapIntensity;

        HDRCubemap::DrawHDRCubemap(viewMatrix, projectionMatrix, intensity);

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
            shader->setVec3("g_DirLight.ambient", *directionalLight->Color * *directionalLight->Intensity);
            shader->setVec3("g_DirLight.diffuse", *directionalLight->Color * *directionalLight->Intensity);
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
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].ambient", *light->Color * *light->Intensity);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].diffuse", *light->Color * *light->Intensity);
            shader->setVec3("g_PointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f) * *light->Intensity);
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

    void RenderSystem::BloomDownSampling(unsigned int texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        m_BloomDownSampleShader->use();
        glm::vec2 baseResolution = {m_BloomFrameBuffer->colorAttachments[0].width,
                                    m_BloomFrameBuffer->colorAttachments[0].height};

        m_BloomDownSampleShader->setVec2("srcResolution", baseResolution);

        for (size_t i = 0; i < m_BloomMipChainAttachments.size(); ++i)
        {
            ColorAttachment mip = m_BloomMipChainAttachments[i];

            glViewport(0, 0, mip.width, mip.height);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *mip.texture, 0);

            glBindVertexArray(m_RenderScreenQuad.VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderScreenQuad.EBO);
            glDrawElements(GL_TRIANGLES, m_RenderScreenQuad.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            m_BloomDownSampleShader->setVec2("srcResolution", {mip.width, mip.height});
            glBindTexture(GL_TEXTURE_2D, *mip.texture);
        }
    }

    void RenderSystem::BloomUpSampling(float filterRadius)
    {
        m_BloomUpSampleShader->use();
        m_BloomUpSampleShader->setFloat("filterRadius", filterRadius);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);

        for (unsigned int i = m_BloomMipChainAttachments.size() - 1; i > 0; i--)
        {
            ColorAttachment mip = m_BloomMipChainAttachments[i];
            ColorAttachment nextMip = m_BloomMipChainAttachments[i - 1];

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *mip.texture);

            glViewport(0, 0, nextMip.width, nextMip.height);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *nextMip.texture, 0);

            glBindVertexArray(m_RenderScreenQuad.VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderScreenQuad.EBO);

            glDrawElements(GL_TRIANGLES, m_RenderScreenQuad.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        glDisable(GL_BLEND);
    }
}
