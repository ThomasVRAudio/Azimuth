#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Editor/ImGuiStyling.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Editor/EditorPropertiesPanel.h>
#include <Azimuth/Editor/EditorHierarchyPanel.h>
#include <Azimuth/Project/Serializer.h>
#include <dependencies/imgui/ImGuizmo.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    class Scene;

    class EditorUI
    {
    public:
        static void Init(Scene *scene);
        static void DrawUI();
        static void DrawEditorScene(unsigned int *texture, Camera &camera);

        template <typename CallbackFn>
        static void SetLightsUpdateCallback(CallbackFn callback)
        {
            lightUpdateCallback = callback;
        };

        static void UpdateLights()
        {
            if (lightUpdateCallback)
                lightUpdateCallback();
        }

        static void EndDraw();
        static void CreateDocker();
        static void DrawGizmos(Camera &camera);
        static void ReadPixelID(unsigned int framebufferID, unsigned int textureWidth, unsigned int textureHeight, unsigned int attachment);
        inline static ImVec2 GetSceneWindowPos() { return m_SceneWindowPos; }
        inline static ImVec2 GetSceneWindowSize() { return m_SceneWindowSize; }

    private:
        inline static ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        inline static std::function<void()> lightUpdateCallback = nullptr;
        static void SetAspectConstraints(ImGuiSizeCallbackData *data);
        inline static ImGuiWindowFlags m_WindowFlags;
        inline static ImVec2 m_SceneWindowPos;
        inline static ImVec2 m_SceneWindowSize;
        inline static float m_SceneWindowAspectRatio = 1.778f;
        inline static Scene *m_Scene = nullptr;
        inline static Entity m_SelectedEntity = -1;
        friend class EditorHierarchyPanel;
        friend class EditorPropertiesPanel;
    };

}

#endif