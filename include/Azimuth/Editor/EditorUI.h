#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Editor/ImGuiStyling.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Editor/EditorPropertiesPanel.h>
#include <Azimuth/Editor/EditorHierarchyPanel.h>
#include <Azimuth/Project/Serializer.h>

namespace Azimuth
{
    class Scene;

    class EditorUI
    {
    public:
        static void Init(Scene *scene);
        static void DrawUI();
        static void DrawEditorScene(unsigned int *texture);

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
        inline static ImVec2 GetSceneWindowPos() { return m_SceneWindowPos; }
        inline static ImVec2 GetSceneWindowSize() { return m_SceneWindowSize; }

    private:
        inline static std::function<void()> lightUpdateCallback = nullptr;
        static void SetAspectConstraints(ImGuiSizeCallbackData *data);
        inline static ImGuiIO *io = nullptr;
        inline static ImGuiWindowFlags m_WindowFlags;
        inline static ImVec2 m_SceneWindowPos;
        inline static ImVec2 m_SceneWindowSize;
        inline static float m_SceneWindowAspectRatio = 1.778f;
        inline static Scene *m_Scene = nullptr;
        inline static Entity m_SelectedEntity;
        friend class EditorHierarchyPanel;
        friend class EditorPropertiesPanel;
    };

}

#endif