#pragma once

namespace Azimuth
{
    class FrameBuffer
    {
    public:
        static void CreateFramebuffer(unsigned int *framebuffer, unsigned int *texture,
                                      unsigned int width, unsigned int height, bool enableStencilDepth = true);
        static void BindFramebuffer(unsigned int *framebuffer);
    };

}