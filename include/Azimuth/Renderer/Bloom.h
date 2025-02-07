#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Renderer/Shader.h>
#include <Azimuth/Renderer/FrameBuffer.h>
#include <Azimuth/Renderer/Geometry.h>

namespace Azimuth
{
    class Bloom
    {
    public:
        Bloom(unsigned int width, unsigned int height);
        void RenderPass(unsigned int texture, float filterRadius, unsigned int *blendTexture = nullptr, float blendMix = 0.5);
        inline unsigned int GetTexture() { return *m_BloomFrameBuffer->colorAttachments[0].texture; }

    private:
        unsigned int m_Width, m_Height;
        void DownSampling(unsigned int texture);
        void UpSampling(float filterRadius);
        void Blend(unsigned int *texture, float blendMix);
        float m_FilterRadius;
        std::vector<ColorAttachment> m_BloomMipChainAttachments;
        std::unique_ptr<FrameBufferConfig> m_BloomFrameBuffer;
        std::unique_ptr<Shader> m_BloomDownSampleShader = nullptr;
        std::unique_ptr<Shader> m_BloomUpSampleShader = nullptr;
        std::unique_ptr<Shader> m_BlendShader = nullptr;
        unsigned int m_MipChainLength = 6;
        unsigned int mipTextures[6];
        Mesh m_RenderScreenQuad = Geometry::Screen();
    };
}