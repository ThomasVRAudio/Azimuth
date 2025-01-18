#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void RenderSystem::Init()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
    }

    void RenderSystem::DrawScene(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ECSManager &ECS = ECSManager::GetInstance();
        for (auto &entity : m_Entities)
        {
            MeshComponent &mesh = ECS.GetComponent<MeshComponent>(entity);
            model = glm::mat4(1.0f);
            if (mesh.shader)
            {
                model = glm::translate(model, ECS.GetComponent<TransformComponent>(entity).Position);

                mesh.shader->use();
                mesh.shader->setMat4("model", model);
                mesh.shader->setMat4("view", viewMatrix);
                mesh.shader->setMat4("projection", projectionMatrix);

                mesh.DrawMesh();
            }
        }
    };

    RenderSystem::~RenderSystem()
    {
        if (activeShader)
            delete activeShader;
    }
}