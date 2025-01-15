#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void RenderSystem::Init()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        // Needs custom width / height
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    }

    void RenderSystem::DrawScene()
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
                mesh.shader->setMat4("view", view);
                mesh.shader->setMat4("projection", projection);

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