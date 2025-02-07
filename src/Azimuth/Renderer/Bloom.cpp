#include <Azimuth/Renderer/Bloom.h>

namespace Azimuth
{

    Bloom::Bloom(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height)
    {
        glm::vec2 currentMipSize = {width, height};

        for (size_t i = 0; i < m_MipChainLength; i++)
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
        m_BlendShader = std::make_unique<Shader>("assets/shaders/default/texcoords.vert", "assets/shaders/default/blend.frag");
    }

    void Bloom::DownSampling(unsigned int texture)
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

    void Bloom::UpSampling(float filterRadius)
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

    void Bloom::RenderPass(unsigned int texture, float filterRadius, unsigned int *blendTexture, float blendMix)
    {
        FrameBuffer::BindFramebuffer(&m_BloomFrameBuffer->ID);
        glClear(GL_COLOR_BUFFER_BIT);
        DownSampling(texture);
        UpSampling(filterRadius);
        if (blendTexture)
            Blend(blendTexture, blendMix);
    }

    void Bloom::Blend(unsigned int *texture, float blendMix)
    {
        m_BlendShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GetTexture());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, *texture);

        m_BlendShader->setInt("g_Texture1", 0);
        m_BlendShader->setInt("g_Texture2", 1);
        m_BlendShader->setFloat("g_Blend", blendMix);

        glBindVertexArray(m_RenderScreenQuad.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderScreenQuad.EBO);
        glDrawElements(GL_TRIANGLES, m_RenderScreenQuad.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

}