#pragma once
#ifdef AZIMUTH_EDITOR
#include <Azimuth/Common.h>
#include <Azimuth/Scene/Scene.h>
#include <dependencies/imgui/ImGuizmo.h>
#include <memory>

namespace Azimuth
{
    class Camera;
    class FrameBufferConfig;
    class SceneSettings;
    class EditorHierarchyPanel;
    class EditorPropertiesPanel;
    class EditorFilepicker;
    class EditorFileTrayPanel;

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
        inline static void RenderGizmos(bool render) { m_RenderGizmos = render; }

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
        inline static bool m_RenderGizmos = true;
        friend class EditorHierarchyPanel;
        friend class EditorPropertiesPanel;
        friend class EditorFilepicker;
        friend class EditorFileTrayPanel;
    };

}

#endif