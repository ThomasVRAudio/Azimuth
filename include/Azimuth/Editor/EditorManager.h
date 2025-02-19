#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Editor/ImGuiStyling.h>
#include <Azimuth/Renderer/Window.h>
#include <Azimuth/Core/Application.h>
#include <Azimuth/Editor/EditorPropertiesPanel.h>
#include <Azimuth/Editor/EditorHierarchyPanel.h>
#include <Azimuth/Editor/EditorSettingsPanel.h>
#include <Azimuth/Editor/EditorFilepicker.h>
#include <Azimuth/Editor/EditorFileTrayPanel.h>
#include <Azimuth/Editor/EditorTextureLoader.h>
#include <Azimuth/Project/Serializer.h>
#include <dependencies/imgui/ImGuizmo.h>
#include <Azimuth/ECS/Component.h>

namespace Azimuth
{
    class Scene;

    class EditorManager
    {
    public:
        static void Init(Scene *scene);
        static void OnUpdate(Camera &camera, FrameBufferConfig *sceneBuffer, FrameBufferConfig *entityBuffer);
        static void DrawUI();
        static void DrawEditorScene(Camera &camera, FrameBufferConfig *sceneBuffer, FrameBufferConfig *entityBuffer);

        template <typename CallbackFn>
        inline static void SetLightsUpdateCallback(CallbackFn callback) { lightUpdateCallback = callback; };
        inline static void UpdateLights()
        {
            if (lightUpdateCallback)
                lightUpdateCallback();
        }

        static void EndDraw();
        static void CreateDocker();
        static void DrawGizmos(Camera &camera);
        static bool CheckMouseHoverScene(double &x, double &y, unsigned int textureWidth, unsigned int textureHeight);
        static int ReadPixelID(FrameBufferConfig *frameBuffer);
        inline static std::shared_ptr<SceneSettings> GetSceneSettings() { return m_SceneSettings; };
        inline static ImVec2 GetSceneWindowPos() { return m_SceneWindowPos; }
        inline static ImVec2 GetSceneWindowSize() { return m_SceneWindowSize; }
        inline static bool IsSceneFocused() { return m_IsSceneWindowFocused; }
        inline static bool IsSceneHovered() { return m_IsSceneWindowHovered; }

    private:
        static void SetAspectConstraints(ImGuiSizeCallbackData *data);
        inline static ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        inline static ImGuiWindowFlags m_WindowFlags;
        inline static std::function<void()> lightUpdateCallback = nullptr;
        inline static std::shared_ptr<SceneSettings> m_SceneSettings = nullptr;
        inline static Scene *m_Scene = nullptr;
        inline static ImVec2 m_SceneWindowPos, m_SceneWindowSize, m_SceneWindowPadding;
        inline static Entity m_SelectedEntity = -1;
        inline static float m_SceneWindowAspectRatio = 1.778f;
        inline static bool m_IsManipulating = false, m_IsSceneWindowFocused = false, m_IsSceneWindowHovered = false, t_HasOpenedScene = false;
        friend class EditorHierarchyPanel;
        friend class EditorPropertiesPanel;
        friend class EditorFilepicker;
        friend class EditorFileTrayPanel;
    };

}

#endif