#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    enum class FrameBufferTextureFormat
    {
        None = 0,
        RGBA = GL_RGBA,
        RED_INTEGER = GL_RED_INTEGER,
        RED_INTEGER_INTERNAL = GL_R32I
    };

    enum class RenderBufferDepthFormat
    {
        None = 0,
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
        STENCIL_INDEX8 = GL_STENCIL_INDEX8,
    };

    enum class DepthAttachmentTarget
    {
        None = 0,
        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };

    struct ColorAttachment
    {
        unsigned int width, height;
        FrameBufferTextureFormat format;
        FrameBufferTextureFormat internalFormat;
        unsigned int *texture;

        ColorAttachment(unsigned int width, unsigned int height,
                        unsigned int *texture = nullptr,
                        FrameBufferTextureFormat format = FrameBufferTextureFormat::RGBA,
                        FrameBufferTextureFormat internalFormat = FrameBufferTextureFormat::RGBA)
            : width(width), height(height), format(format),
              internalFormat(internalFormat), texture(texture) {}
    };

    struct DepthAttachment
    {
        unsigned int width, height;
        RenderBufferDepthFormat internalFormat;
        DepthAttachmentTarget attachmentTarget;

        DepthAttachment(unsigned int width, unsigned int height,
                        RenderBufferDepthFormat internalFormat = RenderBufferDepthFormat::DEPTH24_STENCIL8,
                        DepthAttachmentTarget attachmentTarget = DepthAttachmentTarget::DEPTH_STENCIL)
            : width(width), height(height), internalFormat(internalFormat),
              attachmentTarget(attachmentTarget) {}
    };

    struct FrameBufferConfig
    {
        unsigned int ID;
        std::vector<ColorAttachment> colorAttachments;
        std::vector<DepthAttachment> depthAttachments;

        FrameBufferConfig() {}
        FrameBufferConfig(const ColorAttachment &color)
        {
            colorAttachments.emplace_back(color);
        }

        FrameBufferConfig(const ColorAttachment &color, const DepthAttachment &depth)
        {
            colorAttachments.emplace_back(color);
            depthAttachments.emplace_back(depth);
        }

        FrameBufferConfig(const std::vector<ColorAttachment> &colors)
        {
            for (auto &color : colors)
            {
                colorAttachments.emplace_back(color);
            }
        }

        FrameBufferConfig(const std::vector<ColorAttachment> &colors, const DepthAttachment &depth)
        {
            for (const auto &color : colors)
            {
                colorAttachments.emplace_back(color);
            }

            depthAttachments.emplace_back(depth);
        }

        FrameBufferConfig(const std::vector<ColorAttachment> &colors, const std::vector<DepthAttachment> &depths)
        {
            for (const auto &color : colors)
            {
                colorAttachments.emplace_back(color);
            }
            for (const auto &depth : depths)
            {
                depthAttachments.emplace_back(depth);
            }
        }
    };

    class FrameBuffer
    {
    public:
        static void CreateFramebuffer(unsigned int *framebuffer, unsigned int *texture,
                                      unsigned int width, unsigned int height, bool enableStencilDepth = true);
        static void BindFramebuffer(unsigned int *framebuffer);
        static void CreateFramebuffer(FrameBufferConfig *config);
    };

}