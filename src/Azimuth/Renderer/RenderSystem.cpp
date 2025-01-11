#include <Azimuth/Renderer/RenderSystem.h>
#include <dependencies/GLFW/glfw3.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void RenderSystem::Init()
    {
        ECSManager &ECS = ECSManager::GetInstance();
    }

    void RenderSystem::DrawScene()
    {
        ECSManager &ECS = ECSManager::GetInstance();
        for (auto &entity : m_Entities)
        {
        }
    };
}