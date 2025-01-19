#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void RenderSystem::Init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);

        std::vector<std::string> faces = {
            "assets/skybox/ice/right.jpg",
            "assets/skybox/ice/left.jpg",
            "assets/skybox/ice/top.jpg",
            "assets/skybox/ice/bottom.jpg",
            "assets/skybox/ice/front.jpg",
            "assets/skybox/ice/back.jpg"};

        // m_CubemapId = Cubemap::LoadCubemap(faces);
        HDRCubemap::LoadHDRCubemap("assets/hdr/qwantani.hdr");
    }

    void RenderSystem::DrawScene(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Cubemap::DrawCubemap(m_CubemapId, viewMatrix, projectionMatrix);
        HDRCubemap::DrawHDRCubemap(viewMatrix, projectionMatrix);

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