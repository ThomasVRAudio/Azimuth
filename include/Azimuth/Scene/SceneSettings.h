#pragma once

namespace Azimuth
{
    struct SceneSettings
    {
        float Exposure = 1.0f;
        float HDRCubemapIntensity = 1.0f;
        float BloomThreshold = 1.0f;
        float BloomBlend = 0.5f;
        bool VSync = true;
    };
}