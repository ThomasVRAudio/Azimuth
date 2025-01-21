#include <Azimuth/Renderer/RenderSystem.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{

    void RenderSystem::Init(ECSManager *ECS)
    {
        this->ECS = ECS;

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

        HDRCubemap::LoadHDRCubemap("assets/hdr/CasualDay4K.hdr", 4096);
    }

    void RenderSystem::DrawScene(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        HDRCubemap::DrawHDRCubemap(viewMatrix, projectionMatrix);

        for (auto &entity : m_Entities)
        {
            MeshComponent &mesh = ECS->GetComponent<MeshComponent>(entity);
            model = glm::mat4(1.0f);

            if (mesh.shader)
            {
                TransformComponent &transform = ECS->GetComponent<TransformComponent>(entity);
                model = glm::translate(model, transform.Position);
                model = glm::rotate(model, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, ECS->GetComponent<TransformComponent>(entity).Scale);

                mesh.shader->use();
                mesh.shader->setMat4("model", model);
                mesh.shader->setMat4("view", viewMatrix);
                mesh.shader->setMat4("projection", projectionMatrix);

                // To improve
                if (ECS->HasComponent<MaterialComponent>(entity))
                {
                    MaterialComponent &material = ECS->GetComponent<MaterialComponent>(entity);
                    if (material.type == "solid")
                        mesh.shader->setVec3("Color", material.color);
                }

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