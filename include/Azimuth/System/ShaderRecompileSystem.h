#pragma once
#include <Azimuth/ECS/System.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/ECS/Components/MaterialComponent.h>

namespace Azimuth
{
    class ShaderRecompileSystem : public System
    {
    public:
        ShaderRecompileSystem() = default;
        void Init(Scene *scene);
        void CheckAndUpdateShaderChanges();

    private:
        Scene *m_Scene;
        std::unordered_map<std::string, std::filesystem::file_time_type> m_TimeMap;
    };

}