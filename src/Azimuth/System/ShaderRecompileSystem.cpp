#include <Azimuth/System/ShaderRecompileSystem.h>
#include <Azimuth/Scene/Scene.h>

namespace Azimuth
{
    void ShaderRecompileSystem::Init(Scene *scene)
    {
        m_Scene = scene;
    }

    void ShaderRecompileSystem::CheckAndUpdateShaderChanges()
    {
        if (m_Scene == nullptr)
        {
            print("Shader Recompile System not Initialized");
            return;
        }

        for (const auto &entity : m_Entities)
        {
            MaterialComponent &component = m_Scene->GetComponent<MaterialComponent>(entity);
            std::filesystem::path shaderPath = component.shader->GetPaths().first;
            auto newTime = std::filesystem::last_write_time(shaderPath);

            if (m_TimeMap.find(shaderPath.string()) == m_TimeMap.end())
                m_TimeMap[shaderPath.string()] = newTime;

            if (m_TimeMap[shaderPath.string()] != newTime)
            {
                component.shader->ReloadShader();
                component.UpdateUniforms();
                m_TimeMap[shaderPath.string()] = newTime;
            }
        }
    }
}