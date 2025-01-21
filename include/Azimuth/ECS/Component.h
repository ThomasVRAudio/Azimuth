#pragma once
#include <Azimuth/Common.h>
#include <Azimuth/Scene/Scene.h>
#include <Azimuth/Renderer/PrimitiveMeshes.h>
#include <Azimuth/Renderer/Shader.h>

namespace Azimuth
{

    struct IComponent
    {
    };

    struct TransformComponent : public IComponent
    {
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Rotation = glm::vec3(0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);
    };

    enum PRIMITIVE_TYPE
    {
        None = 0,
        PRIMITIVE_POINT = 1,
        PRIMITIVE_LINE = 2,
        PRIMITIVE_TRIANGLE = 3,
        PRIMITIVE_SQUARE = 4,
        PRIMITIVE_PLANE = 5,
        PRIMITIVE_CUBE = 6,
        PRIMITIVE_SPHERE = 7
    };

    class MaterialComponent : public IComponent
    {
    public:
        std::string type = "solid";
        glm::vec3 color;
    };

    class MeshComponent : public IComponent
    {
    public:
        MeshComponent();
        ~MeshComponent();

        void CreateMesh(PRIMITIVE_TYPE primitive, std::shared_ptr<Shader> shader = nullptr);
        void CreateMesh(const std::vector<float> &verts, std::shared_ptr<Shader> shader);
        void UpdateMeshPrimitive(PRIMITIVE_TYPE primitive);
        void DrawMesh();

        std::shared_ptr<Shader> shader;
        inline PRIMITIVE_TYPE GetMeshType() { return m_Type; };

    private:
        void GenerateBuffers();
        void SetBufferData(PRIMITIVE_TYPE primitive);
        PRIMITIVE_TYPE m_Type = None;
        unsigned int m_VAO, m_VBO, m_EBO;
        std::vector<float> m_Vertices;
        std::vector<int> m_Indices;
    };

    struct AudioComponent : public IComponent
    {
    };

    class MonoScript;
    class Scene;

    class ScriptsComponent : public IComponent
    {
    public:
        ScriptsComponent() = default;

        void AddScript(std::shared_ptr<MonoScript> script)
        {
            m_Scripts.emplace_back(script);
        }

        void OnStart();
        void OnUpdate();

    private:
        std::vector<std::shared_ptr<MonoScript>> m_Scripts;
        Scene *m_Scene;
        Entity m_Entity;
        friend MonoScript;
        friend Scene;
    };

    class MonoScript : public IComponent
    {
    public:
        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
        GetComponent()
        {
            return m_ScriptParent->m_Scene->GetComponent<T>(m_ScriptParent->m_Entity);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, void>::type
        AddComponent()
        {
            T component;
            return m_ScriptParent->m_Scene->AddComponent<T>(m_ScriptParent->m_Entity, component);
        }

        template <typename T>
        typename std::enable_if<std::is_base_of<IComponent, T>::value, void>::type
        RemoveComponent()
        {
            return m_ScriptParent->m_Scene->RemoveComponent<T>(m_ScriptParent->m_Entity);
        }

        void SetParent(std::shared_ptr<ScriptsComponent> parent)
        {
            m_ScriptParent = parent;
        }

        bool HasParent()
        {
            return m_ScriptParent != nullptr;
        }

    private:
        std::shared_ptr<ScriptsComponent> m_ScriptParent;
    };

    struct TagComponent : public IComponent
    {
        std::string name;
    };
}