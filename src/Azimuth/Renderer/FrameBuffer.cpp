#include <Azimuth/Renderer/FrameBuffer.h>

namespace Azimuth
{

    void FrameBuffer::CreateFramebuffer(unsigned int *framebuffer, unsigned int *texture,
                                        unsigned int width, unsigned int height, bool enableStencilDepth)
    {
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

        if (enableStencilDepth)
        {
            unsigned int rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Framebuffer not complete: " << status << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::CreateFramebuffer(FrameBufferConfig *config)
    {
        glGenFramebuffers(1, &config->ID);
        glBindFramebuffer(GL_FRAMEBUFFER, config->ID);

        std::set<unsigned int> textureIDs;

        GLuint attachments[config->colorAttachments.size()];

        for (size_t i = 0; i < config->colorAttachments.size(); ++i)
        {
            ColorAttachment &attachment = config->colorAttachments[i];
            if (textureIDs.find(*attachment.texture) == textureIDs.end())
            {
                glGenTextures(1, attachment.texture);
                textureIDs.insert(*attachment.texture);
                glBindTexture(GL_TEXTURE_2D, *attachment.texture);

                glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(attachment.internalFormat), attachment.width, attachment.height,
                             0, static_cast<GLenum>(attachment.format), GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                {
                    std::cerr << "OpenGL Create Framebuffer Error at: " << i << ": " << error << std::endl;
                }
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, *attachment.texture);
            }

            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, *attachment.texture, 0);
        }
        glDrawBuffers(config->colorAttachments.size(), attachments);

        for (size_t i = 0; i < config->depthAttachments.size(); ++i)
        {
            DepthAttachment &attachment = config->depthAttachments[i];
            unsigned int rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(attachment.internalFormat), attachment.width, attachment.height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(attachment.attachmentTarget), GL_RENDERBUFFER, rbo);
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Framebuffer not complete: " << status << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::BindFramebuffer(unsigned int *framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        int width, height;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

        glViewport(0, 0, width, height);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Bind Framebuffer failed: " << status << std::endl;
        }
    }

}